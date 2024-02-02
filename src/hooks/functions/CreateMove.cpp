#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"

#include "../../sdk/classes/CBasePlayer.h"



void __fastcall hooks::create_move::hooked(CSGOInput* input, int slot, bool unk, bool sub_tick)
{
    sdk::sub_tick = sub_tick;
    sdk::user_cmd = input->GetUserCmd();
    sdk::base_user_cmd = sdk::user_cmd->m_base_cmd;

    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame() || !entity_data::local_player_pawn || !sdk::user_cmd || !sdk::base_user_cmd)
        original_fn(input, slot, unk, sub_tick);

    original_fn(input, slot, unk, sub_tick);
}