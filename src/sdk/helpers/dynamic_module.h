#pragma once
#include <string>
#include <optional>
#include <string>
#include <Windows.h>
#include <cstdint>
#include <map>
#include <vector>
#include "../helpers/importer.h"

using fn = void* (__cdecl*)();

struct InterfaceReg
{
	fn func;
	const char* name;
	InterfaceReg* next;
};

//Usage:
//modules::client.pattern_scan("").abs().add(0x1).as<void*>();

class PatternScanner
{
public:
	PatternScanner(HMODULE base)
	{
		this->base = base;
	}

	PatternScanner()
	{

	}

	PatternScanner& scan(const char* signature)
	{
		this->addr = pattern_scan(signature);

		return *this;
	}

	PatternScanner& abs()
	{
		const int relative = *reinterpret_cast<int*>(this->addr);

		this->addr += relative + sizeof(int);

		return *this;
	}

	PatternScanner& add(const uintptr_t& offset)
	{
		this->addr += offset;

		return *this;
	}

	template <typename T = void>
	T* as()
	{
		if (!addr)
			return {};

		return reinterpret_cast<T*>(addr);
	}
private:
	std::uint8_t* pattern_scan(const char* signature) const
	{
		static auto pattern_to_byte = [](const char* pattern)
			{
				auto bytes = std::vector<int>{};
				auto start = const_cast<char*>(pattern);
				auto end = const_cast<char*>(pattern) + strlen(pattern);

				for (auto current = start; current < end; ++current)
				{
					if (*current == '?')
					{
						++current;

						if (*current == '?')
							++current;

						bytes.emplace_back(-1);
					}
					else bytes.emplace_back(strtoul(current, &current, 16));
				}
				return bytes;
			};

		auto offset_ptr = [](auto* base, std::size_t offset) -> PIMAGE_NT_HEADERS
			{
				return reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(base) + offset);
			};

		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(this->base);
		auto nt_headers = offset_ptr(dos_header, dos_header->e_lfanew);

		auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_byte(signature);
		auto scan_bytes = reinterpret_cast<uint8_t*>(this->base);

		for (std::size_t i = 0; i < size_of_image - pattern_bytes.size(); ++i)
		{
			bool found = true;
			for (std::size_t j = 0; j < pattern_bytes.size(); ++j)
			{
				if (scan_bytes[i + j] != pattern_bytes[j] && pattern_bytes[j] != -1)
				{
					found = false;
					break;
				}
			}

			if (found)
				return &scan_bytes[i];
		}

		return nullptr;
	}

	const char* pattern;
	HMODULE base;
	uint8_t* addr;
};

class DynamicModule
{
public:
	std::map<std::string, HMODULE> modules = {};

	HMODULE get_module(const std::string& name)
	{
		if (modules.count(name) == 0 || !modules[name])
			modules[name] = LI_FN(GetModuleHandleA).cached()(name.c_str());

		return modules[name];
	}

	DynamicModule(const std::string_view& dll_name)
	{
		this->base = get_module(dll_name.data());
		this->base_addr = reinterpret_cast<uintptr_t>(this->base);
		this->dll_name = dll_name.data();

		pattern_scanner = PatternScanner(this->base);
	}

	DynamicModule()
	{

	}

	~DynamicModule()
	{
		//FreeLibrary(dll);
	}

	void* GetExport(const std::string_view& func_name)
	{
		if (!base)
			return nullptr;

		return LI_FN(GetProcAddress).cached()(base, func_name.data());
	}

	void* GetCreateInterface()
	{
		void* CreateInterface = this->GetExport("CreateInterface");

		if (!CreateInterface)
			return nullptr;

		return CreateInterface;
	}

	template<typename T>
	T GetInterface(const std::string_view& interface_name)
	{
		using fn = void* (*)(const char* pName, int* pReturnCode);

		fn CreateInterface = reinterpret_cast<fn>(GetCreateInterface());

		if (CreateInterface)
		{
			T interface_val = reinterpret_cast<T>(CreateInterface(interface_name.data(), 0));

			return interface_val;
		}

		return nullptr;
	}

	template<typename T>
	T GetInterfaceFromList(const std::string_view& interface_name)
	{
		uint8_t* CreateInterface = reinterpret_cast<uint8_t*>(GetCreateInterface());

		if (!CreateInterface)
			return nullptr;

		const auto& rip_offset = resolve_rip(CreateInterface, std::nullopt, std::nullopt);

		InterfaceReg* reg = *reinterpret_cast<InterfaceReg**>(rip_offset);

		if (!reg)
			return nullptr;

		for (InterfaceReg* it = reg; it; it = it->next)
		{
			size_t pos = interface_name.find(it->name);
			if (pos != std::string_view::npos)
			{
				T interface_val = reinterpret_cast<T>(it->func());

				return interface_val;
			}
		}

		return nullptr;
	}

	void PrintAllInterfaces()
	{
		uint8_t* CreateInterface = reinterpret_cast<uint8_t*>(GetCreateInterface());

		if (!CreateInterface)
			return;

		InterfaceReg* reg = *reinterpret_cast<InterfaceReg**>(resolve_rip(CreateInterface, std::nullopt, std::nullopt));

		if (reg)
		{
			printf("reg: 0x%p", reg);

			for (InterfaceReg* it = reg; it; it = it->next)
			{
				printf("name: %s\n", it->name);
			}
		}
	}

	static uint8_t* resolve_rip_copy(std::uint8_t* addr, std::uint32_t rva, std::uint32_t size)
	{
		auto relative = addr + *reinterpret_cast<int*>(addr + rva);

		return reinterpret_cast<uint8_t*>(relative + size);
	}

	static uint8_t* resolve_rip(uint8_t* address, std::optional<size_t> offset = std::nullopt, std::optional<size_t> length = std::nullopt)
	{
		uint32_t displacement = *reinterpret_cast<uint32_t*>(address + (offset.value_or(0x3)));

		return address + (length.value_or(0x7)) + static_cast<size_t>(displacement);
	}

	HMODULE base;
	uintptr_t base_addr;
	const char* dll_name;
	PatternScanner pattern_scanner{};
};





