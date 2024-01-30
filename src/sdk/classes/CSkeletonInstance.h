#pragma once
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/classes/CGameSceneNode.h"
#include "../../sdk/classes/CBoneSystem.h"

class CSkeletonInstance //: public CGameSceneNode //Compiler error because of line #4: include loop 
{
public:
	NETVAR(CModelState, m_modelState, "CSkeletonInstance", "m_modelState");
	NETVAR(uint8_t, m_nHitboxSet, "CSkeletonInstance", "m_nHitboxSet");
};
