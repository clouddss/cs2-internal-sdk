#include "Console.h"

#include "../helpers/importer.h"

namespace console
{
    HANDLE out, old_out;

    void attach()
    {
        AllocConsole();
        AttachConsole(ATTACH_PARENT_PROCESS);

//#ifdef NDEBUG
//        ShowWindow(GetConsoleWindow(), SW_SHOWMINIMIZED);
//#endif //NDEBUG

        ShowWindow(GetConsoleWindow(), SW_MINIMIZE);

        FILE* file;
        freopen_s(&file, "CONIN$", "r", stdin);
        freopen_s(&file, "CONOUT$", "w", stdout);
        freopen_s(&file, "CONOUT$", "w", stderr);
    }

    void detach()
    {
        HWND hwnd = GetConsoleWindow();

        FreeConsole();

        PostMessageA(hwnd, WM_QUIT, 0, 0); //Was WM_CLOSE
    }
}