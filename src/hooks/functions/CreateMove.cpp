#include "../hooks.h"
#include "../../sdk/sdk.h"

#include "../../sdk/classes/CBasePlayer.h"

void __fastcall hooks::create_move::hooked(CSGOInput* input, unsigned int a2, void* a3, unsigned __int8 unk)
{
    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
        original_fn(input, a2, a3, unk);

    //printf("Hello from create_move\n");

    original_fn(input, a2, a3, unk);
}