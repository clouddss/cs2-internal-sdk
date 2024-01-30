#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CGameSceneNode.h"
#include "../../sdk/classes/CEntityInstance.h"

enum LifeState_t : uint8_t
{
    LIFE_ALIVE,
    LIFE_DYING,
    LIFE_DEAD,
    LIFE_RESPAWNABLE,
    LIFE_RESPAWNING
};

class CCollisionProperty //TODO: Add more members later
{
public:
    NETVAR(Vector, m_vecMins, "CCollisionProperty", "m_vecMins");
    NETVAR(Vector, m_vecMaxs, "CCollisionProperty", "m_vecMaxs");
};

class CBaseEntity : public CEntityInstance
{
public:
	NETVAR(int, m_iHealth, "C_BaseEntity", "m_iHealth");
	NETVAR(LifeState_t, m_lifeState, "C_BaseEntity", "m_lifeState");
    NETVAR(uint8_t, m_iTeamNum, "C_BaseEntity", "m_iTeamNum");
    NETVAR(CGameSceneNode*, m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
    NETVAR(CCollisionProperty*, m_pCollision, "C_BaseEntity", "m_pCollision"); //This returns nullptr for CBasePlayer, wtf?

    /*CCollisionProperty* m_pCollision()
    {
        return reinterpret_cast<CCollisionProperty*>(reinterpret_cast<uintptr_t>(this) + 0x320);
    }*/
};


