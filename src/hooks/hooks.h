#pragma once
#include "../sdk/hooking/minhook/MinHook.h"
#include "../sdk/classes/CEntityInstance.h"
#include "../sdk/classes/CHandle.h"
#include "../sdk/math/Viewmatrix.h"
#include "../sdk/sdk.h"
#include <d3d11.h>

class CDetourHook {
	void* function;
	void* detour;
	void* trampoline;
public:
	void hook(void* target, void* dtr) {
		function = target;
		detour = dtr;

		MH_CreateHook(function, detour, &trampoline);
	}

	void hook(uintptr_t target, void* dtr) {
		hook((void*)target, dtr);
	}

	template <typename tOriginal>
	tOriginal get_original() {
		return reinterpret_cast<tOriginal>(trampoline);
	}

	void unhook() {
		if (!function || !detour)
			return;

		MH_DisableHook(function);
		MH_RemoveHook(function);
	}
};

template<typename tFunc = void*>
inline tFunc get_vfunction(void* class_ptr, int index) {
	void** __vfptr = *reinterpret_cast<void***>(class_ptr);
	return reinterpret_cast<tFunc>(__vfptr[index]);
}

namespace hooks
{
	bool init();
	bool detach();

	namespace wndproc
	{
		inline WNDPROC original;
		LRESULT __stdcall hooked(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	struct should_draw_player
	{
		using fn = bool(__fastcall*)(void*);
		static bool __fastcall hooked(void* player_pawn);

		inline static fn original_fn;
	};

	struct create_move
	{
		static const int index = 5;
		using fn = void(__fastcall*)(c_csgo_input*, int, bool, bool);
		static void __fastcall hooked( c_csgo_input* input, int slot, bool unk, bool sub_tick);

		inline static fn original_fn;
	};

	struct on_add_entity
	{
		static const int index = 14;
		using fn = CEntityInstance*(__thiscall*)(void*, CEntityInstance*, CHandle);
		static CEntityInstance* __fastcall hooked(void* rcx, CEntityInstance* instance, CHandle handle);

		inline static fn original_fn;
	};

	struct on_remove_entity
	{
		static const int index = 15;
		using fn = CEntityInstance * (__thiscall*)(void*, CEntityInstance*, CHandle);
		static CEntityInstance* __fastcall hooked(void* rcx, CEntityInstance* instance, CHandle handle);

		inline static fn original_fn;
	};

	struct present
	{
		static const int index = 8;
		using fn = long(__stdcall*)(void*, uint32_t, uint32_t);
		static long __stdcall hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags);

		inline static fn original_fn;
	};

	struct resize_buffers
	{
		static const int index = 13;
		using fn = long(__stdcall*)(void*, uint32_t, uint32_t, uint32_t, DXGI_FORMAT, uint32_t);
		static long __stdcall hooked(IDXGISwapChain* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags);

		inline static fn original_fn;
	};

	struct frame_stage_notify
	{
		using fn = void(__fastcall*)(void*, int);
		static void __fastcall hooked(void* a1, int stage);

		inline static fn original_fn;
	};

	struct fov_changer_test
	{
		using fn = float(__fastcall*)(void*);
		static float __fastcall hooked(void* camera);

		inline static fn original_fn;
	};
	
	struct get_matrices_for_view
	{
		using fn = void(__fastcall*)(void*, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels);
		static void __fastcall hooked(void* rcx, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels);

		inline static fn original_fn;
	};

	extern ID3D11Device* device;
	extern ID3D11DeviceContext* context;
	extern ID3D11RenderTargetView* render_target_view;
	extern ID3D11Texture2D* back_buffer;
	extern HWND window;
}
