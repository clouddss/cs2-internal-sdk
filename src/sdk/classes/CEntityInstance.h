#pragma once
#include "../../sdk/classes/CEntityIdentity.h"
#include "../../sdk/interfaces/SchemaSystem.h"
//#include "../../sdk/classes/CHandle.h"

class CEntityInstance
{
public:
    CEntityIdentity* m_pEntity()
    {
        return reinterpret_cast<CEntityIdentity*>(reinterpret_cast<uintptr_t>(this) + 0x10);
    }

    SchemaClassInfoData_t* GetSchemaClassInfo()
    {
        SchemaClassInfoData_t* pClassInfo = nullptr;
        GetVirtual<void(__thiscall*)(void*, SchemaClassInfoData_t**)>(this, 34)(this, &pClassInfo);

        return pClassInfo;
    }

    bool IsController()
    {
        const auto& class_info = GetSchemaClassInfo();

        if (!class_info)
            return false;

        auto hash = fnv::hash_runtime(class_info->m_name);

        return hash == FNV("CCSPlayerController");
    }

    bool IsPawn()
    {
        const auto& class_info = GetSchemaClassInfo();

        if (!class_info)
            return false;

        auto hash = fnv::hash_runtime(class_info->m_name);

        return hash == FNV("C_CSPlayerPawn");
    }
};
