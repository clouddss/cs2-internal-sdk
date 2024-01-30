#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/classes/CSkeletonInstance.h"

class CGameSceneNode
{
public:
    NETVAR(Vector, m_vecOrigin, "CGameSceneNode", "m_vecOrigin");
    NETVAR(Vector, m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin");

    CSkeletonInstance* GetSkeletonInstance()
    {
        return GetVirtual<CSkeletonInstance * (__thiscall*)(void*)>(this, 8)(this);
    }
};

