#pragma once

#include "GameEntitySystem.h"

class CGameResourceService
{
public:
	CGameEntitySystem* GetGameEntitySystem()
	{
		return *reinterpret_cast<CGameEntitySystem**>(reinterpret_cast<uintptr_t>(this) + 0x58);
	}
};
