// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>// Windows Header Files
#include "sysinfo_com.h"
#include "factory.h"
#include "registry.h"
#include <memory>

//---------------------------------------------------------------   
// Global Variables
static HMODULE g_hModule = nullptr; // dll module handle
long g_cServerLocks = 0; // Server locks counter
long g_cComponents = 0; // components counter
//--------------------------------------------------------------- 

extern "C" __declspec( dllexport ) IUnknown* CreateInstance()
{
   trace( "CreateInstance(). %d components. %d serverLocks\n", g_cComponents, g_cServerLocks );
   return new CSysInfo;
}
//---------------------------------------------------------------

__control_entrypoint( DllExport ) STDAPI DllRegisterServer( void )
{
   return RegisterServer( g_hModule, CLSID_SysInfo_Component, g_szFriendlyName, g_szVerIndProgID, g_szProgID );
}
//---------------------------------------------------------------

__control_entrypoint( DllExport ) STDAPI DllUnregisterServer( void )
{
   return UnregisterServer( CLSID_SysInfo_Component, g_szVerIndProgID, g_szProgID );
}
//---------------------------------------------------------------

//_Check_return_
STDAPI  DllGetClassObject( _In_ REFCLSID clsid, _In_ REFIID iid, _Outptr_ LPVOID FAR* ppv )
{
   trace( "DllGetClassObject(). %d components. %d server locks. \n", g_cComponents, g_cServerLocks );
   if( clsid == CLSID_SysInfo_Component )// Is it our component?
   {
      CFactory* pFactory(new CFactory); // creating the factory
      if( pFactory )
         return pFactory->QueryInterface( iid, ppv );
      else
         return E_OUTOFMEMORY;
   } else
      return CLASS_E_CLASSNOTAVAILABLE;
}
//---------------------------------------------------------------
STDAPI DllCanUnloadNow()
{
   trace( "DllCanUnloadNow(). %d components. %d server locks. \n", g_cComponents, g_cServerLocks );
   if( ( g_cComponents == 0 ) && ( g_cServerLocks == 0 ) )
      return S_OK;
   else
      return S_FALSE;
}
//---------------------------------------------------------------

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
   trace( "DLLMain(). %d components. %d server locks. ", g_cComponents, g_cServerLocks );
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
       g_hModule = hModule; 
       trace( "attach to a process\n" ); 
       break;
    case DLL_PROCESS_DETACH: 
       assert( g_cComponents==0 && g_cServerLocks==0 ); //memory leaks?
       trace( "detach from the process\n" ); 
    //   break;
    //case DLL_THREAD_ATTACH:
    //case DLL_THREAD_DETACH:
    //    break;
    }
    return TRUE;
}
//---------------------------------------------------------------

