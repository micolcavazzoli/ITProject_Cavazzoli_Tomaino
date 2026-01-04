#include "pch.h"
#include "LookbackOption.h"
#include "pricer_api.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: // The DLL is being loaded into the virtual address space
    case DLL_THREAD_ATTACH: // A new thread is being created in the current process
    case DLL_THREAD_DETACH: // A thread in the current process is exiting cleanly
    case DLL_PROCESS_DETACH: // The DLL is being unloaded from the virtual address space
        break;
    }
    return TRUE; // Return TRUE to indicate successful initialization
}

