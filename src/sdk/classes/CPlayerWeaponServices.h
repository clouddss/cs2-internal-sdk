#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CHandle.h"
#include "../../sdk/classes/CBasePlayerWeapon.h"

class CPlayerWeaponServices
{
public:
    NETVAR(CHandle, m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon");
};

