   #pragma once
#include "../classes/CEntityInstance.h"
#include "../helpers/modules.h"
#include "../classes/CBaseEntity.h"

#include <WinDNS.h>
//#include "../classes/CHandle.h"

class CGameEntitySystem
{
public:
	CBaseEntity* GetBaseEntity(uint32_t index)
	{
		uint64_t entity;

		uint64_t temp_entity_ptr = *(uint64_t*)(this + 0x8 * (index >> 9) + 0x10);

		if (!temp_entity_ptr)
			return nullptr;

		int64_t temp_entity = 0x78 * (index & 0x1FF) + temp_entity_ptr;

		if (!temp_entity)
			return nullptr;

		uint32_t entity_index = (*(uint32_t*)(temp_entity + 0x10) & 0x7FFF);

		if (index <= 0x7FFE && (index >> 9) <= 0x3F && temp_entity_ptr && temp_entity && entity_index == index)
			entity = *(uint64_t*)temp_entity;
		else entity = 0;

		/*if (entity && reinterpret_cast<CBaseEntity*>(entity)->IsBasePlayerController())
			printf("entity_index: %d\n", entity_index);*/

		return reinterpret_cast<CBaseEntity*>(entity);
	}

	/*CBaseEntity* GetBaseEntity(int index)
	{
		using fn = CBaseEntity*(__thiscall*)(void*, int);

		static const auto& addr = modules::client.pattern_scan("8B D3 E8 ? ? ? ? 48 8B F8 48 85 C0 74 76");

		if (!addr)
			return nullptr;

		//DynamicModule::resolve_rip_copy(addr, 3, 0);

		auto rip_offset = DynamicModule::resolve_rip(addr, 0x3, std::nullopt);

		if (!rip_offset)
			return nullptr;

		auto get_base_entity_fn = reinterpret_cast<fn>(rip_offset);

		if (!get_base_entity_fn)
			return nullptr;

		const auto& base_entity = get_base_entity_fn(this, index);

		return base_entity;
	}*/

	CEntityInstance* FindEntityByName(const char* name)
	{
		return GetVirtual<CEntityInstance*(__thiscall*)(void*, const char*)>(this, 12)(this, name);
	}

	CEntityInstance* OnAddEntity(CEntityInstance* entity_instance, void* handle)
	{
		return GetVirtual<CEntityInstance*(__thiscall*)(void*, CEntityInstance*, void*)>(this, 14)(this, entity_instance, handle);
	}

	CEntityInstance* OnRemoveEntity(CEntityInstance* entity_instance, void* handle)
	{
		return GetVirtual<CEntityInstance * (__thiscall*)(void*, CEntityInstance*, void*)>(this, 15)(this, entity_instance, handle);
	}

	int GetHighestEntityIndex()
	{
		return *reinterpret_cast<int*>(this + 0x1510);
	}
};