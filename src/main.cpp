#include <memory>
#include "sdk/sdk.h"
#include "sdk/classes/CBasePlayer.h"
#include "sdk/helpers/netvars.h"
#include "sdk/helpers/console.h"
#include "hooks/hooks.h"
#include "sdk/helpers/entity_data.h"
#include "sdk/classes/CBasePlayerWeapon.h"
#include "sdk/math/Vector.h"

//Notice: This project is WORK IN PROGRESS!
DWORD __stdcall on_attach(void* base)
{
    if (!modules::nav_system.base)
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //Sleep 5 seconds to avoid switch to desktop because of console
    
    console::attach(); //TODO: Minimize console if in release mode? //#ifdef NDEBUG #endif // NDEBUG

    sdk::init_modules();
    sdk::init_interfaces();

    netvars::init();
    hooks::init();

    while (!sdk::can_unhook)
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

    //TODO: if (sdk::can_unhook) {};
    hooks::detach();
    console::detach();

    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(base), EXIT_SUCCESS);
  
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, void* reserved)
{
    if (reason != DLL_PROCESS_ATTACH)
        return false;
    
    DisableThreadLibraryCalls(module);

    auto thread = LI_FN(CreateThread)(nullptr, 0, on_attach, module, 0, nullptr);

    if (thread)
        LI_FN(CloseHandle)(thread);

    return TRUE;
}

