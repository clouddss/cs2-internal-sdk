#include "sdk.h"
#include "../sdk/helpers/modules.h"
#include "../hooks/hooks.h"

#include <d3d11.h>

void print_status(const char* name, void* ptr)
{
	printf("%s: 0x%p\n", name, ptr);
}

namespace sdk
{
	inline ID3D11Device* device;
	bool sub_tick;
	c_usercmd* user_cmd;
	c_base_usercmd* base_user_cmd;
	inline ID3D11DeviceContext* context;
	inline ID3D11RenderTargetView* render_target_view;

	VMatrix* viewmatrix;

	bool can_unhook = false;

	bool world_to_screen(const Vector& pos, Vector& out) 
	{
		if (!viewmatrix)
			return false;

		out.x = viewmatrix->m[0][0] * pos.x + viewmatrix->m[0][1] * pos.y + viewmatrix->m[0][2] * pos.z + viewmatrix->m[0][3];
		out.y = viewmatrix->m[1][0] * pos.x + viewmatrix->m[1][1] * pos.y + viewmatrix->m[1][2] * pos.z + viewmatrix->m[1][3];

		float w = viewmatrix->m[3][0] * pos.x + viewmatrix->m[3][1] * pos.y + viewmatrix->m[3][2] * pos.z + viewmatrix->m[3][3];

		if (w < 0.01f)
			return false;

		float inv_w = 1.f / w;
		out.x *= inv_w;
		out.y *= inv_w;

		int width, height;
		g::engine_client->GetScreenSize(width, height);

		float x = width  * .5f;
		float y = height * .5f;

		x += 0.5f * out.x * width + 0.5f;
		y -= 0.5f * out.y * height + 0.5f;

		out.x = x;
		out.y = y;

		return true;
	}

	void init_modules()
	{
		modules::client = DynamicModule("client.dll");
		modules::nav_system = DynamicModule("navsystem.dll");
		modules::engine = DynamicModule("engine2.dll");
		modules::schema = DynamicModule("schemasystem.dll");
		modules::tier0 = DynamicModule("tier0.dll");
		modules::render_dx11 = DynamicModule("rendersystemdx11.dll");

	}

	void init_interfaces()
	{
		g::engine_client = modules::engine.GetInterfaceFromList<IVEngineClient*>("Source2EngineToClient001");
		g::schema_system = modules::schema.GetInterfaceFromList<CSchemaSystem*>("SchemaSystem_001");
		g::game_resource_service = modules::engine.GetInterfaceFromList<CGameResourceService*>("GameResourceServiceClientV001");

		g::csgo_input = g::csgo_input->get();
		g::entity_system = g::game_resource_service->GetGameEntitySystem();
		g::render_system = **reinterpret_cast<CRenderSystem***>(modules::render_dx11.pattern_scanner.scan("66 0F 7F 05 ? ? ? ? 66 0F 7F 0D ? ? ? ? 48 89 35 ? ? ? ?").add(4).abs().as<uint8_t>());

		if (g::render_system)
			g::swap_chain = g::render_system->swap_chain;

		g::mem_alloc = *reinterpret_cast<IMemAlloc**>(modules::tier0.GetExport("g_pMemAlloc"));

		printf("Interfaces info:\n");
		print_status("engine_client", g::engine_client);
		print_status("schema_system", g::schema_system);
		print_status("mem_alloc", g::mem_alloc);
		print_status("game_resource_service", g::game_resource_service);
		print_status("entity_system", g::entity_system);
		print_status("csgo_input", g::entity_system);
		print_status("render_system", g::render_system);
		print_status("swap_chain", g::swap_chain);
		printf("\n");
	}
}

namespace interfaces
{
	IVEngineClient* engine_client{};
	CSchemaSystem* schema_system{};
	IMemAlloc* mem_alloc{};
	CGameResourceService* game_resource_service{};
	CGameEntitySystem* entity_system{};
	c_csgo_input* csgo_input{};
	CRenderSystem* render_system{};
	IDXGISwapChain* swap_chain{};
}

