#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>// Windows Header Files
#include "Iface.h"
#include <string>

//---------------------------------------------------------------
class CSysInfo : public ISysInfo
{
public:
   // IUnknown
   virtual HRESULT   __stdcall QueryInterface( const IID& iid, void** ppv );
   virtual ULONG     __stdcall AddRef();
   virtual ULONG     __stdcall Release();
   //ISysInfo
   virtual HRESULT   __stdcall OSversion( char** pResult ) override;
   virtual HRESULT   __stdcall MBvendor ( char** pResult ) override;
   virtual HRESULT   __stdcall CPUcores ( char** pResult ) override;
   virtual HRESULT   __stdcall monitors ( char** pResult ) override;

   CSysInfo();
   ~CSysInfo();
private:
   long m_cRef;   //reference counter
};
//---------------------------------------------------------------

// Global Variables
extern const TCHAR g_szFriendlyName[];// Component Friendly name
extern const TCHAR g_szVerIndProgID[];// ProgID w/o version
extern const TCHAR g_szProgID[];// ProgID
extern long g_cComponents; // Количество активных компонентов

//---------------------------------------------------------------
//utility functions
#define TRACE2DEBUGGER //comment this if it's needed to trace to cout.
#ifdef _DEBUG
inline void trace( const std::string& szData )
{
#ifdef TRACE2DEBUGGER
   _CrtDbgReport( _CRT_WARN, nullptr, NULL, nullptr, "%s", szData.c_str() );
#else
   std::cout << szData;
#endif
}
//---------------------------------------------------------------
inline void trace( const std::wstring& szData )
{
#if defined(_WINBASE_)
   _CrtDbgReportW( _CRT_WARN, nullptr, NULL, nullptr, L"%ls", szData.c_str() );
#else
   std::wcout << szData;
#endif
}
//---------------------------------------------------------------
inline void trace( LPCSTR lpszFormat, ... )
{
   va_list args;
   va_start( args, lpszFormat );
   std::string szBuffer( vsnprintf( nullptr, 0, lpszFormat, args ) + 1, '\0' );
   vsnprintf( &szBuffer.front(), szBuffer.size(), lpszFormat, args );
   va_end( args );

   trace( szBuffer );
}
//---------------------------------------------------------------
inline void trace( LPCWSTR lpszFormat, ... )
{
   va_list args;
   va_start( args, lpszFormat );
   std::wstring szBuffer( vswprintf( nullptr, 0, lpszFormat, args ) + 1, '\0' );
   vswprintf( &szBuffer.front(), szBuffer.size(), lpszFormat, args );
   va_end( args );

   trace( szBuffer );
}
//---------------------------------------------------------------
#else
inline void trace(...){};
inline void assert( ... ){};
#endif


