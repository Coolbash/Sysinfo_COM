// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <atlbase.h>

#include "Iface.h"

#ifdef _DEBUG
void trace( const char* msg ) { std::cout << "client: \t\t" << msg << std::endl; };
#else
void trace( const char* msg ) {};
#endif

typedef IUnknown* ( *CREATEFUNCPTR )( );
//---------------------------------------------------------------
IUnknownPtr CallCreateInstance( LPCSTR name )
{
   HINSTANCE hComponent = ::LoadLibraryA( name );
   if( hComponent == NULL )
   {
      std::cout << "Can't load the component: " <<name<< std::endl;
      return NULL;
   }

   CREATEFUNCPTR CreateInstance = ( CREATEFUNCPTR )::GetProcAddress( hComponent, "CreateInstance" );
   if( CreateInstance == NULL )
   {
      std::cout << "CallCreateInstance:\tError: cant find CreateInstance"<< std::endl;
      return NULL;
   }
   return CreateInstance();
}
//---------------------------------------------------------------
int main()
{
   CoInitialize( NULL );
   {
      ISysInfoPtr pSysInfo;// ( CLSID_SysInfo_Component, nullptr, CLSCTX_INPROC_SERVER );   //smart pointer. No need to Release();
      HRESULT hr = CoCreateInstance( CLSID_SysInfo_Component, NULL, CLSCTX_INPROC_SERVER, __uuidof( ISysInfo ), ( void** )& pSysInfo );
      if( FAILED( hr ) )
      {
         std::wcout << _T("failed to open interface by Class Factory: ") << _com_error(hr).ErrorMessage() << _T("... Trying to open through dll...\n");
         IUnknownPtr pI = CallCreateInstance( "sysinfo_com.dll" );
         if( pI )
         {
            std::cout << "successful.\n";
            hr = pI->QueryInterface( __uuidof( ISysInfo ), ( void** )& pSysInfo );
         }else 
            return 0;
      }
      if( SUCCEEDED( hr ) )
      {
         CComHeapPtr< char> szOSvrsion, szMBvendor, szCPUcoures, szMonitors; //smart pointers. No need to CoTaskMemFree()
                                                                             //OS version
         hr = pSysInfo->OSversion( &szOSvrsion );
         if( SUCCEEDED( hr ) && szOSvrsion )
            std::cout << "OS version: " << szOSvrsion << std::endl;
         else
            std::wcout << _T( "OSversion() failed: " ) << _com_error( hr ).ErrorMessage() << std::endl;

         //MotherBorard name and vendor
         hr = pSysInfo->MBvendor( &szMBvendor );
         if( SUCCEEDED( hr ) && szMBvendor )
            std::cout << "MotherBoard: " << szMBvendor << std::endl;
         else
            std::wcout << _T( "MBvendor() failed: " ) << _com_error( hr ).ErrorMessage() << std::endl;

         //CPUcores
         hr = pSysInfo->CPUcores( &szCPUcoures );
         if( SUCCEEDED( hr ) && szCPUcoures )
            std::cout << "CPU: " << szCPUcoures << std::endl;
         else
            std::wcout << _T( "CPUcores() failed: " ) << _com_error( hr ).ErrorMessage() << std::endl;

         //monitors
         hr = pSysInfo->monitors( &szMonitors );
         if( SUCCEEDED( hr ) && szMonitors )
            std::cout << "Monitors:\n" << szMonitors << std::endl;
         else
            std::wcout << _T( "monitors() failed: " ) << _com_error( hr ).ErrorMessage() << std::endl;
      }
      else
         std::wcout << _T( "failed: " ) << _com_error( hr ).ErrorMessage() << std::endl;
   }
   CoUninitialize();
   return 0;
}
//---------------------------------------------------------------
//---------------------------------------------------------------