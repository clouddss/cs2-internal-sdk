#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../render/menu/main_window.h"

namespace hooks
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* render_target_view;
	ID3D11Texture2D* back_buffer;
	HWND window;

	namespace esp //Just an example, dont put any next features here! Make an esp.cpp
	{
		std::list<entity_data::player_data_t> m_entries; //get list of player_data_t so we can get the player data

		void render(ImDrawList* draw_list)
		{
			if (!g::engine_client->IsInGame())
				return;

			if (entity_data::player_instances.empty())
				return;

			if (entity_data::locker.try_lock()) //mutex stuff
			{
				m_entries.clear();
				std::copy(entity_data::player_entry_data.front().player_data.begin(), entity_data::player_entry_data.front().player_data.end(), std::back_inserter(m_entries));
				entity_data::locker.unlock();
			}

			Vector screen_head_pos;
			Vector screen_player_pos;
			Vector head_pos;
			Vector out;
			Vector bone_pos_out;
			Vector bone_parent_pos_out;

			for (auto& data : m_entries) //TODO:Compute bbox in the future
			{
				if (data.index == 0)
					continue;

				head_pos = data.m_vOldOrigin;
				head_pos.z += 75.f;

				if (!sdk::world_to_screen(data.m_vOldOrigin, screen_player_pos))
					continue;

				if (!sdk::world_to_screen(head_pos, screen_head_pos))
					continue;

				if (!sdk::world_to_screen(data.m_vecOrigin, out))
					continue;

				float height = screen_player_pos.y - screen_head_pos.y;
				float width = height * 2.5f;
				width /= 10.f;

				// Example text
				draw_list->AddText(ImVec2(out.x, out.y), IM_COL32_WHITE, std::to_string(data.m_iHealth).c_str()); 

				// Example box
				draw_list->AddRect({ screen_head_pos.x - width, screen_head_pos.y }, { screen_head_pos.x + width, screen_player_pos.y }, IM_COL32_WHITE, 0.f, 0.f, 1);

				const auto& bbox = data.bbox;

				const bool& on_screen = (bbox.left > 0 || bbox.right > 0) && (bbox.top > 0 || bbox.bottom > 0);
				
				printf("on_screen: %d, left: %d, right: %d, top: %d, bottom: %d\n", on_screen, bbox.left, bbox.right, bbox.top, bbox.bottom);

				if (settings::visuals::m_bBoneEsp)
				{
					if (!data.model_state)
						continue;

					if (data.model.IsValid())
					{
						for (int i = 0; i <= data.model->BoneCount; ++i)
						{
							const auto flag = data.model->GetBoneFlags(i);

							if (!flag.HasFlag(static_cast<uint32_t>(FLAG_HITBOX)))
								continue;

							auto bone_parent_index = data.model->GetBoneParent(i);

							if (bone_parent_index == -1)
								continue;

							if (sdk::world_to_screen(data.model_state->bones[i].position, bone_pos_out) &&
								sdk::world_to_screen(data.model_state->bones[bone_parent_index].position, bone_parent_pos_out))
							{
								draw_list->AddLine(ImVec2{ bone_pos_out.x, bone_pos_out.y }, ImVec2{ bone_parent_pos_out.x, bone_parent_pos_out.y }, IM_COL32_WHITE);
							}
						}
					}
				}
			}
		}
	}

	std::once_flag init_flag;
	long __stdcall present::hooked(IDXGISwapChain* swap_chain, uint32_t sync_interval, uint32_t flags)
	{
		//Todo: cleanup imgui and the various dx11 pointers on exit/unhook, etc...

		DXGI_SWAP_CHAIN_DESC desc;
		swap_chain->GetDesc(&desc);

		window = desc.OutputWindow;
		
		std::call_once(init_flag, [&]() 
		{
			swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device));
			device->GetImmediateContext(&context);
			swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));

			device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);

			if (back_buffer)
				back_buffer->Release();

			hooks::wndproc::original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc::hooked)));

			ImGui::CreateContext();
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(device, context);

			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui::GetStyle().WindowMinSize = ImVec2(500, 350);
		});

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		{
			if (main_window::is_open)
				main_window::Draw();

			ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

			esp::render(draw_list);
		}
		ImGui::EndFrame();

		ImGui::Render();
		context->OMSetRenderTargets(0, nullptr, nullptr);
		context->OMSetRenderTargets(1, &render_target_view, nullptr);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return original_fn(swap_chain, sync_interval, flags);
	}
}