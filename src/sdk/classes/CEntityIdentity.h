#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/helpers/CUtlSymbolLarge.hpp"

class CEntityIdentity
{
public:
    NETVAR(CUtlSymbolLarge, m_name, "CEntityIdentity", "m_name");
    NETVAR(CUtlSymbolLarge, m_designerName, "CEntityIdentity", "m_designerName");

    //int m_nameStringableIndex; // 0x14 int32_t
    //CUtlSymbolLarge m_name; // 0x18 CUtlSymbolLarge
    //CUtlSymbolLarge m_designerName; // 0x20 CUtlSymbolLarge
    //uint32_t m_flags; // 0x30 uint32_t
    //void* m_worldGroupId; // 0x38 WorldGroupId_t
    //uint32_t m_fDataObjectTypes; // 0x3C uint32_t
    //void* m_PathIndex; // 0x40 ChangeAccessorFieldPathIndex_t
    //CEntityIdentity* m_pPrev; // 0x58 CEntityIdentity*
    //CEntityIdentity* m_pNext; // 0x60 CEntityIdentity*
    //CEntityIdentity* m_pPrevByClass; // 0x68 CEntityIdentity*
    //CEntityIdentity* m_pNextByClass; // 0x70 CEntityIdentity*
};
