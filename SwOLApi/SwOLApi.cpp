// SwOLApi.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch( ul_reason_for_call ) 
    { 

		case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
			//OutputDebugString("DLL_PROCESS_ATTACH");
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
			//OutputDebugString("DLL_THREAD_ATTACH");
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
			//OutputDebugString("DLL_THREAD_DETACH");
            break;

        case DLL_PROCESS_DETACH:
			//OutputDebugString("DLL_PROCESS_DETACH");
         // Perform any necessary cleanup.
            break;
	}


	return TRUE;
}


