#pragma once
#include <list>

#include "../classes/CBasePlayer.h"
#include "../classes/CSkeletonInstance.h"
#include "../classes/CGameSceneNode.h"
#include "../classes/CStrongHandle.h"

#include <mutex>

namespace entity_data
{
	struct player_data_t
	{
		int m_iHealth;
		Vector m_vecOrigin;
		int m_iShotsFired;
		int clip;
		uint32_t index;
		Vector m_vOldOrigin;
		CModelState* model_state;
		CStrongHandle<CModel> model;
		RECT bbox;

		Vector mins;
		Vector maxs;
	};

	struct entry_data_t
	{
		std::list<player_data_t> player_data;
	};

	struct instance_t
	{
		CBasePlayer* entity;
		CHandle handle;
	};

	extern std::list<instance_t> player_instances;
	extern std::list<instance_t> entity_instances;
	extern std::list<entry_data_t> player_entry_data;

	extern std::mutex locker;

	void fetch_player_data();
	void destroy();
}

