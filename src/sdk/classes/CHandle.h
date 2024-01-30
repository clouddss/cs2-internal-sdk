#pragma once
#include "../sdk.h"
#include "../../sdk/classes/CBaseEntity.h"

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF

class CHandle
{
public:
    bool operator==(const CHandle& rhs) const
    {
        return entity_index == rhs.entity_index;
    }

    bool IsValid() const
    {
        return entity_index != INVALID_EHANDLE_INDEX;
    }

    int GetEntryIndex() const
    {
        return entity_index & ENT_ENTRY_MASK;
    }

    template <typename T = CBaseEntity>
    T* Get() const
    {
        auto base_entity = g::entity_system->GetBaseEntity(GetEntryIndex());

        return reinterpret_cast<T*>(base_entity);
    }

    uint32_t entity_index;
};