#include "hooks.h"
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"
#include <algorithm>

bool hooks::init()
{
	if (MH_Initialize() != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::entity_system, on_add_entity::index, &on_add_entity::hooked, reinterpret_cast<void**>(&on_add_entity::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::entity_system, on_remove_entity::index, &on_remove_entity::hooked, reinterpret_cast<void**>(&on_remove_entity::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::csgo_input, create_move::index, &create_move::hooked, reinterpret_cast<void**>(&create_move::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHook(modules::client.pattern_scanner.scan("48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A").as(), &frame_stage_notify::hooked, reinterpret_cast<void**>(&frame_stage_notify::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHook(modules::client.pattern_scanner.scan("E8 ? ? ? ? F3 0F 11 45 ? 48 8B 5C 24 ?").add(0x1).abs().as(), &fov_changer_test::hooked, reinterpret_cast<void**>(&fov_changer_test::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::swap_chain, present::index, &present::hooked, reinterpret_cast<void**>(&present::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHookVirtual(g::swap_chain, resize_buffers::index, &resize_buffers::hooked, reinterpret_cast<void**>(&resize_buffers::original_fn)) != MH_STATUS::MH_OK)
		return false;

	if (MH_CreateHook(modules::client.pattern_scanner.scan("40 53 48 81 EC ? ? ? ? 49 8B C1").as(), get_matrices_for_view::hooked, reinterpret_cast<void**>(&get_matrices_for_view::original_fn)) != MH_STATUS::MH_OK)
		return false;

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

bool hooks::detach()
{
	SetWindowLongPtrA(hooks::window, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

	if (MH_Uninitialize() != MH_STATUS::MH_OK)
		return false;

	return true;
}

float __fastcall hooks::fov_changer_test::hooked(void* camera)
{
	if (g::engine_client->IsInGame() && settings::visuals::m_bFovChanger)
		return static_cast<float>(settings::visuals::m_iFov);

    return original_fn(camera);
}

//Temp placement, move later
void __fastcall hooks::get_matrices_for_view::hooked(void* rcx, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels)
{
	if (!sdk::viewmatrix)
		sdk::viewmatrix = world_to_projection;

	original_fn(rcx, rdx, world_to_view, view_to_projection, world_to_projection, world_to_pixels);
}

long __stdcall hooks::resize_buffers::hooked(IDXGISwapChain* swap_chain, uint32_t buffer_count, uint32_t width, uint32_t height, DXGI_FORMAT new_format, uint32_t swap_chain_flags)
{
	const auto hr = original_fn(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
	
	if (hr >= 0)
	{
		ImGui_ImplDX11_CreateDeviceObjects();
		ImGui_ImplDX11_InvalidateDeviceObjects();
		ImGui::CreateContext();

		swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));
		device->GetImmediateContext(&context);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(device, context);

		swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));

		// Create render target view
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
	}

	return hr;
}

