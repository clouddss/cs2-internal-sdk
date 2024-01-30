#include "../hooks.h"
#include "../../sdk/sdk.h"

#include "../../sdk/classes/CBasePlayer.h"

#include "../../sdk/helpers/entity_data.h"

void __fastcall hooks::frame_stage_notify::hooked(void* a1, int stage)
{
	switch (stage) 
	{
	case FRAME_START:
		break;

	case FRAME_NET_UPDATE_START:
		entity_data::fetch_player_data();
		break;

	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		break;

	case FRAME_NET_UPDATE_END:
		break;

	case FRAME_RENDER_START:
		break;

	case FRAME_RENDER_END:
		break;

	case FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE:
		break;
	}

	original_fn(a1, stage);
}
