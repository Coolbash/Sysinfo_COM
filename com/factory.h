#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>// Windows Header Files
#include "Iface.h"

//---------------------------------------------------------------
class CFactory : public IClassFactory// Class factory
{
public:
   // IUnknown
   virtual HRESULT __stdcall QueryInterface( const IID& iid, void** ppv );
   virtual ULONG __stdcall AddRef();
   virtual ULONG __stdcall Release();
   // IClassFactory
   virtual HRESULT __stdcall CreateInstance( IUnknown* pUnknownOuter, const IID& iid, void** ppv );
   virtual HRESULT __stdcall LockServer( BOOL bLock );

   CFactory(); //constructor
   ~CFactory();// destructor
private:
   long m_cRef;//reference counter
};
//--------------------------------------------------------------- 

// Global Variables
extern const TCHAR g_szFriendlyName[];// Component Friendly name
extern const TCHAR g_szVerIndProgID[];// ProgID w/o version
extern const TCHAR g_szProgID[];// ProgID
extern long g_cServerLocks; 
extern long g_cComponents; 

//---------------------------------------------------------------
