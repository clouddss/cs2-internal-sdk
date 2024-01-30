#include "CBasePlayer.h"

#include "../../sdk/helpers/fnv.h"

bool CBasePlayer::InAir()
{
    return m_hGroundEntity() == nullptr;
}

bool CBasePlayer::IsAlive()
{
    return m_iHealth() > 0 || m_lifeState() == LIFE_ALIVE;
}


