#pragma once

#include "../helpers/vfunc.h"

enum EClientFrameStage : int
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END,
    FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class IVEngineClient
{
public:

    void ExecuteClientCmd(const char* command) //not working
    {
        GetVirtual<void(__thiscall*)(void*, const char*)>(this, 36)(this, command); // index != 38 37 36
    }

    bool IsPaused() //not returning correct values
    {
        return GetVirtual<bool(__thiscall*)(void*)>(this, 11)(this); //not 10 11 12 13   15 16 17 18
    }

    int GetMaxClients()
    {
        return GetVirtual<int(__thiscall*)(void*)>(this, 31)(this);
    }

    bool IsInGame()
    {
        return GetVirtual<bool(__thiscall*)(void*)>(this, 32)(this);
    }

    bool IsConnected()
    {
        return GetVirtual<bool(__thiscall*)(void*)>(this, 33)(this);
    }

    void GetScreenSize(int& width, int& height)
    {
        GetVirtual<void(__thiscall*)(void*, int&, int&)>(this, 50)(this, width, height);
    }

    const char* GetLevelName()
    {
        return GetVirtual<const char* (__thiscall*)(void*)>(this, 53)(this);
    }

    const char* GetLevelNameShort()
    {
        return GetVirtual<const char* (__thiscall*)(void*)>(this, 54)(this);
    }

    int GetLocalPlayer()
    {
        return GetVirtual<int(__thiscall*)(void*)>(this, 84)(this);
    }
};