#include <algorithm>
#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"

void remove_players(CEntityInstance* instance, CHandle handle)
{
    if (!instance || !instance->IsController())
        return;
    
   auto it = std::remove_if(entity_data::player_instances.begin(), entity_data::player_instances.end(), [&](const entity_data::instance_t& e) { return e.entity->m_pEntity() == instance->m_pEntity(); });

   entity_data::player_instances.erase(it, entity_data::player_instances.end());
}

void on_remove_entity_fn(CEntityInstance* instance, CHandle handle)
{
    if (!instance)
        return;

    remove_players(instance, handle);
}

CEntityInstance* __fastcall hooks::on_remove_entity::hooked(void* rcx, CEntityInstance* instance, CHandle handle)
{
    on_remove_entity_fn(instance, handle);

    return original_fn(rcx, instance, handle);
}


