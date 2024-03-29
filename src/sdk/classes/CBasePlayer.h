#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CPlayerWeaponServices.h"

//TODO: Rebuild the entity classes, since nonlocalplayer entities/players are CCSPlayerControllers

class CCSPlayerController_InGameMoneyServices
{
public:
    NETVAR(int, m_iAccount, "CCSPlayerController_InGameMoneyServices", "m_iAccount");
};

class CBasePlayerPawn
{
public:
    NETVAR(CPlayerWeaponServices*, m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
    NETVAR(Vector, m_vOldOrigin, "C_BasePlayerPawn", "m_vOldOrigin");
};

class CCSPlayerController {
public:
    NETVAR(int, m_nFinalPredictedTick, "CBasePlayerController", "m_nFinalPredictedTick");
    NETVAR(char*, m_iszPlayerName, "CBasePlayerCOntroller", "m_iszPlayerName");
    NETVAR(uint64_t, m_steamID, "CBasePlayerController", "m_steamID");
    NETVAR(bool, m_bIsLocalPlayerController, "CBasePlayerController", "m_bIsLocalPlayerController");
    NETVAR(int, m_iDesiredFOV, "CBasePlayerController", "m_iDesiredFOV");
    NETVAR(bool, m_bPawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive");
    NETVAR(const char*, m_sSanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName");
};

class CBasePlayer : public CBaseEntity //Rebuild CBasePlayer class to CCSPlayerController?
{
public:
    NETVAR(const char*, m_iszPlayerName, "CBasePlayerController", "m_iszPlayerName");
    NETVAR(uint32_t, m_fFlags, "C_BaseEntity", "m_fFlags");
    NETVAR(void*, m_hGroundEntity, "C_BaseEntity", "m_hGroundEntity");
    NETVAR(int, m_iShotsFired, "C_CSPlayerPawnBase", "m_iShotsFired");
    NETVAR(Vector, m_aimPunchAngle, "C_CSPlayerPawn", "m_aimPunchAngle");
    NETVAR(int, m_iEFlags, "C_BaseEntity", "m_iEFlags");
    NETVAR(float, m_flFlashDuration, "C_CSPlayerPawnBase", "m_flFlashDuration");
    NETVAR(int, m_iIDEntIndex, "C_CSPlayerPawnBase", "m_iIDEntIndex");
    NETVAR(Vector, m_vOldOrigin, "C_BasePlayerPawn", "m_vOldOrigin");

    NETVAR(CHandle, m_hPawn, "CBasePlayerController", "m_hPawn");

    NETVAR(CPlayerWeaponServices*, m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
    NETVAR(CCSPlayerController_InGameMoneyServices*, m_pInGameMoneyServices, "CCSPlayerController", "m_pInGameMoneyServices");

    bool InAir();
    bool IsAlive();
};

