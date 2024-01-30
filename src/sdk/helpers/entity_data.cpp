#include "entity_data.h"

#include "../../sdk/classes/CStrongHandle.h"

#include <array>

namespace entity_data
{
	std::list<instance_t> player_instances;
	std::list<instance_t> entity_instances;
	std::list<entry_data_t> player_entry_data;

	std::mutex locker;

	void destroy() //TODO: Call this if localplayer is nullptr
	{
		locker.lock();
		{
			player_instances.clear();
		}
		locker.unlock();
	}

	RECT get_box(CBaseEntity* entity)
	{
		auto collision = entity->m_pCollision();

		if (!collision)
			return {};

		auto& mins = collision->m_vecMins();
		auto& maxs = collision->m_vecMaxs();

		static Vector screen_points[8];
		
		//Vector transformed_points[8] =
		//{
		//	{mins.x, mins.y, mins.z}, //minimum extent of the bounding box (bottom-left-back corner)
		//	{mins.x, mins.y, maxs.z}, //minimum x and y extents, but maximum z extent (bottom-left-front corner)
		//	{mins.x, maxs.y, mins.z}, //minimum x extent, maximum y extent, and minimum z extent (bottom-right-back corner)
		//	{mins.x, maxs.y, maxs.z}, //minimum x extent, maximum y extent, and maximum z extent (bottom-right-front corner)
		//	{maxs.x, mins.y, mins.z}, //maximum x extent, minimum y extent, and minimum z extent (top-left-back corner)
		//	{maxs.x, mins.y, maxs.z}, //maximum x extent, minimum y extent, and maximum z extent (top-left-front corner)
		//	{maxs.x, maxs.y, mins.z}, //maximum x and y extents, but minimum z extent (top-right-back corner)
		//	{maxs.x, maxs.y, maxs.z}  //maximum extent of the bounding box (top-right-front corner)
		//};

		for (int i = 0; i < 8; i++)
		{
			Vector transformed_points[8] =
			{
				{
					i & 1 ? maxs.x : mins.x,
					i & 2 ? maxs.y : mins.y,
					i & 4 ? maxs.z : mins.z
				},  // minimum extent of the bounding box (bottom-left-back corner)
				{
					i & 1 ? maxs.x : mins.x,
					i & 2 ? maxs.y : mins.y,
					i & 4 ? maxs.z : maxs.z
				},  // minimum x and y extents, but maximum z extent (bottom-left-front corner)
				{
					i & 1 ? maxs.x : mins.x,
					i & 2 ? maxs.y : maxs.y,
					i & 4 ? maxs.z : mins.z
				},  // minimum x extent, maximum y extent, and minimum z extent (bottom-right-back corner)
				{
					i & 1 ? maxs.x : mins.x,
					i & 2 ? maxs.y : maxs.y,
					i & 4 ? maxs.z : maxs.z
				},  // minimum x extent, maximum y extent, and maximum z extent (bottom-right-front corner)
				{
					i & 1 ? maxs.x : maxs.x,
					i & 2 ? maxs.y : mins.y,
					i & 4 ? maxs.z : mins.z
				},  // maximum x extent, minimum y extent, and minimum z extent (top-left-back corner)
				{
					i & 1 ? maxs.x : maxs.x,
					i & 2 ? maxs.y : mins.y,
					i & 4 ? maxs.z : maxs.z
				},  // maximum x extent, minimum y extent, and maximum z extent (top-left-front corner)
				{
					i & 1 ? maxs.x : maxs.x,
					i & 2 ? maxs.y : maxs.y,
					i & 4 ? maxs.z : mins.z
				},  // maximum x and y extents, but minimum z extent (top-right-back corner)
				{
					i & 1 ? maxs.x : maxs.x,
					i & 2 ? maxs.y : maxs.y,
					i & 4 ? maxs.z : maxs.z
				}   // maximum extent of the bounding box (top-right-front corner)
			};

			if (!sdk::world_to_screen(transformed_points[i], screen_points[i]))
				return {};
		}

		auto top = screen_points[0].y;
		auto left = screen_points[0].x;
		auto right = screen_points[0].x;
		auto bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++)
		{
			if (left > screen_points[i].x)
				left = screen_points[i].x;
			if (top > screen_points[i].y)
				top = screen_points[i].y;
			if (right < screen_points[i].x)
				right = screen_points[i].x;
			if (bottom < screen_points[i].y)
				bottom = screen_points[i].y;
		}

		return RECT{ (long)left, (long)top, (long)(right - left), (long)(bottom - top) };
	}

	void fetch_player_data()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(locker);

		entry_data_t entry_data;
	
		for (const auto& instance : player_instances)
		{
			auto player = instance.entity;

			if (!player || !instance.handle.IsValid())
				continue;

			uint32_t index = instance.handle.GetEntryIndex();

			auto pawn = player->m_hPawn().Get<CBasePlayer>();

			if (!pawn)
				continue;

			auto scene_node = pawn->m_pGameSceneNode();

			if (!scene_node)
				continue;

			auto weapon_services = pawn->m_pWeaponServices();

			if (!weapon_services)
				continue;

			auto active_wpn = weapon_services->m_hActiveWeapon().Get<CBasePlayerWeapon>();

			if (!active_wpn)
				continue;

			auto skeleton_instance = scene_node->GetSkeletonInstance();

			if (!skeleton_instance)
				continue;

			auto& model_state = skeleton_instance->m_modelState();

			auto model = model_state.modelHandle;

			auto collision = pawn->m_pCollision();

			if (!collision)
				continue;

			auto& mins = collision->m_vecMins();
			auto& maxs = collision->m_vecMaxs();

			if (pawn->m_iHealth() <= 0 || !(pawn->m_lifeState() == LIFE_ALIVE))
				continue;

			player_data_t player_data;

			player_data.m_iHealth = pawn->m_iHealth();
			player_data.m_iShotsFired = pawn->m_iShotsFired();
			player_data.m_vecOrigin = scene_node->m_vecOrigin();
			player_data.m_vOldOrigin = pawn->m_vOldOrigin();
			player_data.clip = active_wpn->m_iClip1();
			player_data.model_state = &model_state;
			player_data.model = model;
			player_data.bbox = get_box(pawn);
			player_data.index = index;

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.push_front(std::move(entry_data));
	}
}
