#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>// Windows Header Files
#include "sysinfo_com.h"
#include <iostream>
#include "versionhelpers.h"
#include <atlstr.h>
#include "Registry.h" 

//---------------------------------------------------------------

// Global Variables
const TCHAR g_szFriendlyName[] =  _T("System information little component");// Component Friendly name
const TCHAR g_szVerIndProgID[] =  _T("nix.Sysinfo");// ProgID w/o version
const TCHAR g_szProgID[] =        _T("nix.Sysinfo.1");// ProgID


//---------------------------------------------------------------
CSysInfo::CSysInfo() 
   : m_cRef( 0 )
{ 
   InterlockedIncrement( &g_cComponents ); 
   trace( "CSysinfo created. Total %d components\n", g_cComponents ); 
}
//---------------------------------------------------------------

CSysInfo::~CSysInfo()
{ 
   InterlockedDecrement( &g_cComponents ); 
   trace( "CSysinfo destroyed. Total %d components\n", g_cComponents ); 
}

//---------------------------------------------------------------
HRESULT __stdcall CSysInfo::QueryInterface( const IID& iid, void** ppv )
{
   trace( "CSysInfo::QueryInterface(). old count=%d. %d components.\n", m_cRef, g_cComponents );
   if( iid == IID_IUnknown || iid == __uuidof( ISysInfo ) )
   {
      auto res = static_cast< ISysInfo* >( this );
      AddRef();
      *ppv = res;
      return S_OK;
   }else
   {
      trace( "CSysInfo::QueryInterface() ERROR!!! Interface not found\n" );
      *ppv = nullptr;
      return E_NOINTERFACE;
   }
}
//---------------------------------------------------------------
ULONG __stdcall CSysInfo::AddRef()
{
   trace( "CSysInfo::AddRef(). old count=%d. %d components.\n", m_cRef, g_cComponents );
   return InterlockedIncrement( &m_cRef );
}
//---------------------------------------------------------------
ULONG __stdcall CSysInfo::Release()
{
   trace( "CSysInfo::Release(). old Count=%d. %d components.", m_cRef, g_cComponents );
   if( InterlockedDecrement( &m_cRef ) == 0 )
   {
      trace( " new count=0... Killing itself...\n" );
      delete this;
      return 0;
   }
   trace( " new count=%d\n", m_cRef );
   return m_cRef;
}
//---------------------------------------------------------------
HRESULT  __stdcall CSysInfo::OSversion( char** pResult )
{
   trace( "CSysInfo::OSversion(). Count=%d. %d components.\n", m_cRef, g_cComponents );
   CStringA szResult;
   if( IsWindows10OrGreater() )       //Indicates if the current OS version matches, or is greater than, the Windows 10 version.
      szResult = "Windows 10 or gGreater";
   else
      if( IsWindows8Point1OrGreater() )  //Indicates if the current OS version matches, or is greater than, the Windows 8.1 version.
         szResult = "Windows 8.1 or greater";
      else
         if( IsWindows8OrGreater() )        //Indicates if the current OS version matches, or is greater than, the Windows 8 version.
            szResult = "Windows 8.0 or greater";
         else
            if( IsWindowsVistaSP2OrGreater() ) //Indicates if the current OS version matches, or is greater than, the Windows Vista with Service Pack 2 ( SP2 )version.
               szResult = "Windows Vista SP2 or greater";
            else
               if( IsWindowsVistaSP1OrGreater() ) //Indicates if the current OS version matches, or is greater than, the Windows Vista with Service Pack 1 ( SP1 )version.
                  szResult = "Windows Vista SP1 or greater";
               else
                  if( IsWindowsVistaOrGreater() )    //Indicates if the current OS version matches, or is greater than, the Windows Vista version.
                     szResult = "Windows Vista or greater";
                  else
                     if( IsWindows7SP1OrGreater() )     //Indicates if the current OS version matches, or is greater than, the Windows 7 with Service Pack 1 ( SP1 )version.
                        szResult = "Windows 7 SP1 or greater";
                     else
                        if( IsWindows7OrGreater() )        //Indicates if the current OS version matches, or is greater than, the Windows 7 version.
                           szResult = "Windows 7 or greater";
                        else
                           if( IsWindowsXPSP3OrGreater() )    //Indicates if the current OS version matches, or is greater than, the Windows XP with Service Pack 3 ( SP3 )version.
                              szResult = "Windows XP SP3 or greater";
                           else
                              if( IsWindowsXPSP2OrGreater() )    //Indicates if the current OS version matches, or is greater than, the Windows XP with Service Pack 2 ( SP2 )version.
                                 szResult = "Windows XP SP2 or greater";
                              else
                                 if( IsWindowsXPSP1OrGreater() )    //Indicates if the current OS version matches, or is greater than, the Windows XP with Service Pack 1 ( SP1 )version.
                                    szResult = "Windows XP SP1 or greater";
                                 else
                                    if( IsWindowsXPOrGreater() )       //Indicates if the current OS version matches, or is greater than, the Windows XP version.
                                       szResult = "Windows XP or greater";
      if( IsWindowsServer())            //Indicates if the current OS is a Windows Server release.
         szResult.Append(" (Server edition)");
      if( pResult )
      {
         const int size = szResult.GetLength() + 1;
         void* buf = CoTaskMemAlloc( size );
         memcpy( buf, szResult.GetString(), size );
         *pResult = reinterpret_cast< char* >( buf );
      }

      //OSVERSIONINFO info{sizeof info,0,}; GetVersionEx();
      //DWORD ver{ 0 };
      //GetSystemInfo( NULL );
      //GetProductInfo( 10, 0, 0, 0, &ver );
      return S_OK;
};
//---------------------------------------------------------------
HRESULT __stdcall CSysInfo::MBvendor( char** pResult )
{ 
   trace( "CSysInfo::MBvendor(). Count=%d. %d components.\n", m_cRef, g_cComponents );
   const DWORD bufSize = 256;
   char szName[bufSize] = "Unknown";
   char szVendor[bufSize] = "Unknown";
   CStringA szResult;
   DWORD dwSize = bufSize;
   RegGetValueA( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardProduct", RRF_RT_REG_SZ, NULL, szName, &dwSize );
   dwSize = bufSize;
   RegGetValueA( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardManufacturer", RRF_RT_REG_SZ, NULL, szVendor, &dwSize );
   szResult.Format("%s by %s.",szName,szVendor);
   if( pResult )
   {
      const int size = szResult.GetLength() + 1;
      void* buf = CoTaskMemAlloc( size );
      memcpy( buf, szResult.GetString(), size );
      *pResult = reinterpret_cast< char* >( buf );
      return S_OK;
   }
   else
      return E_INVALIDARG;
};
//---------------------------------------------------------------
CStringA GetCPUInfoString()
{
   char CPUBrandString[0x40] = { 0 };
   int CPUInfo[4] = { -1 };
   unsigned    nIds, nExIds;
   const int buf_size = 1024;
   char  buf[buf_size] = { 0 };
   CStringA szResult;

   // __cpuid with an InfoType argument of 0 returns the number of
   // valid Ids in CPUInfo[0] and the CPU identification string in
   // the other three array elements. The CPU identification string is
   // not in linear order. The code below arranges the information 
   // in a human readable form.
   char CPUString[0x20] = { 0 };

   __cpuid( CPUInfo, 0 );
   nIds = CPUInfo[0];
   memset( CPUString, 0, sizeof( CPUString ) );
   *( ( int* )CPUString ) = CPUInfo[1];
   *( ( int* )( CPUString + 4 ) ) = CPUInfo[3];
   *( ( int* )( CPUString + 8 ) ) = CPUInfo[2];

   // Calling __cpuid with 0x80000000 as the InfoType argument
   // gets the number of valid extended IDs.
   __cpuid( CPUInfo, 0x80000000 );
   nExIds = CPUInfo[0];
   memset( CPUBrandString, 0, sizeof( CPUBrandString ) );

   // Get the information associated with each extended ID.
   for( UINT i = 0x80000000; i <= nExIds; ++i )
   {
      __cpuid( CPUInfo, i );
      // Interpret CPU brand string and cache information.
      if( i == 0x80000002 )
         memcpy( CPUBrandString, CPUInfo, sizeof( CPUInfo ) );
      else 
         if( i == 0x80000003 )
            memcpy( CPUBrandString + 16, CPUInfo, sizeof( CPUInfo ) );
         else 
            if( i == 0x80000004 )
               memcpy( CPUBrandString + 32, CPUInfo, sizeof( CPUInfo ) );
   }

   if( nExIds >= 0x80000004 )
      szResult = CStringA( CPUBrandString ).Trim();
   else
      szResult = CStringA( CPUString ).Trim();
   return  szResult;
}
//---------------------------------------------------------------
HRESULT __stdcall CSysInfo::CPUcores( char** pResult )
{ 
   trace( "CSysInfo::CPUcores(). Count=%d. %d components.\n", m_cRef, g_cComponents );
   CStringA szResult = GetCPUInfoString();
   SYSTEM_INFO si{ 0 };
   GetSystemInfo( &si );
   szResult.AppendFormat( " (%d cores)",si.dwNumberOfProcessors );

   if( pResult )
   {
      const int size = szResult.GetLength() + 1;
      void* buf = CoTaskMemAlloc( size );
      memcpy( buf, szResult.GetString(), size );
      *pResult = reinterpret_cast< char* >(buf);
      return S_OK;
   }else
      return E_INVALIDARG;

};
//---------------------------------------------------------------
//BOOL __stdcall Monitorenumproc( HMONITOR hMonitor, HDC hdc, LPRECT pRect, LPARAM pdwData )
//{
//   MONITORINFOEXA mi;
//   mi.cbSize = sizeof( mi );
//   GetMonitorInfoA( hMonitor, &mi );
//   std::cout << mi.szDevice << "(size: " << mi.rcMonitor.right << "*" << mi.rcMonitor.bottom << ")\n";
//   return TRUE;
//}
//---------------------------------------------------------------
HRESULT __stdcall CSysInfo::monitors( char** pResult )
{ 
   trace( "CSysInfo::monitors(). Count=%d. %d components.\n", m_cRef, g_cComponents );
   //GetSystemMetrics()

   //EnumDisplayDevices shows all virtual displays. Even if it doesn't connect to the system (there is no display at all)
   DISPLAY_DEVICEA DisplayDevice{ sizeof DisplayDevice,0, };
   CStringA szResult, szDevice, szMonitor;
   for( DWORD i = 0; EnumDisplayDevicesA( nullptr, i, &DisplayDevice, 0 ); i++ ) //EDD_GET_DEVICE_INTERFACE_NAME
   {
      if( DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP )
      {
         szDevice = DisplayDevice.DeviceString;
         CStringA szDeviceName( DisplayDevice.DeviceName );
         EnumDisplayDevicesA( szDeviceName.GetString(), i, &DisplayDevice, EDD_GET_DEVICE_INTERFACE_NAME );     //EDD_GET_DEVICE_INTERFACE_NAME
         szResult.AppendFormat( "%s @ %s\n", DisplayDevice.DeviceString, szDevice );
         //szDeviceName = DisplayDevice.DeviceName ;
         //EnumDisplayDevicesA( szDeviceName.GetString(), i, &DisplayDevice, 0 );     //EDD_GET_DEVICE_INTERFACE_NAME
      }
   }

   //DWORD dwData{ 0 };
   //std::cout << "diveces from EnumDisplayMonitors():\n";
   //EnumDisplayMonitors( NULL, NULL, &Monitorenumproc, reinterpret_cast<LPARAM>(&dwData) );
   //std::cout << "\n";
   if( pResult )
   {
      const int size = szResult.GetLength() + 1;
      void* buf = CoTaskMemAlloc( size );
      memcpy( buf, szResult.GetString(), size );
      *pResult = reinterpret_cast< char* >( buf );
      return S_OK;
   }
   else
      return E_INVALIDARG;
};
//---------------------------------------------------------------
//---------------------------------------------------------------

