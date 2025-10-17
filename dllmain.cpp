
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "src/InitBase.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        /*

        NOTE:
        Call Sequence:

        CTOR of global/static objects in YRDict.dll
        DllMain(DLL_PROCESS_ATTACH) of YRDict.dll
        EntryPoint of GameMD.exe
        InitBeforeEveryGameMDFunction
        CTOR of global/static objects in GameMD.exe
        WinMain of GameMD.exe
        ...
        Exit of GameMD.exe
        DTOR of global/static objects in GameMD.exe
        UninitAfterEveryGameMDFunction
        ExitProcess

        */
        InitBeforeEveryGameMDFunction_Register();
    }
    return TRUE;
}

