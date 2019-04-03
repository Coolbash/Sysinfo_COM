#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>// Windows Header Files
#include "factory.h"
#include "sysinfo_com.h"

//---------------------------------------------------------------
CFactory::CFactory() //constructor
   : m_cRef( 0 )
{ 
   InterlockedIncrement( &g_cComponents ); 
   trace( "CFactory created %d components. %d serverLocks\n", g_cComponents, g_cServerLocks );
} 
//---------------------------------------------------------------
CFactory::~CFactory() // destructor
{ 
   InterlockedDecrement( &g_cComponents ); 
   trace( "CFactory destroyed %d components. %d serverLocks\n", g_cComponents, g_cServerLocks );
}
//---------------------------------------------------------------
HRESULT __stdcall CFactory::QueryInterface( const IID& iid, void** ppv )
{
   trace( "CFactory::QueryInterface(). Count=%d. %d components. %d serverLocks\n", m_cRef, g_cComponents, g_cServerLocks );
   if( ( iid == IID_IUnknown ) || ( iid == IID_IClassFactory ) )
   {
      *ppv = static_cast< IClassFactory* >( this );
      AddRef();
      return S_OK;
   }else
   {
      trace( "CFactory::QueryInterface() ERROR!!! Interface not found\n" );
      *ppv = nullptr;
      return E_NOINTERFACE;
   }
}
//---------------------------------------------------------------
ULONG __stdcall CFactory::AddRef()
{
   return InterlockedIncrement( &m_cRef );
   trace( "CFactory::CreateInstance(). new count=%d. %d components. %d serverLocks\n", m_cRef, g_cComponents, g_cServerLocks );
}
//---------------------------------------------------------------
ULONG __stdcall CFactory::Release()
{
   trace( "CFactory::Release(). old count=%d. %d components. %d serverLocks\n", m_cRef, g_cComponents, g_cServerLocks );
   if( InterlockedDecrement( &m_cRef ) == 0 )
   {
      delete this;
      return 0;
   }
   return m_cRef;
}
//--------------------------------------------------------------- IClassFactory implementation
HRESULT __stdcall CFactory::CreateInstance( IUnknown* pUnknownOuter,const IID& iid,void** ppv )
{
   trace( "CFactory::CreateInstance(). Count=%d. %d components. %d serverLocks\n", m_cRef, g_cComponents, g_cServerLocks );

   if( pUnknownOuter )// there is no aggregation support
      return CLASS_E_NOAGGREGATION;
   CSysInfo* pA = new CSysInfo; //creating the component
   if( pA )
      return pA->QueryInterface( iid, ppv );//requested interface
   else 
      return E_OUTOFMEMORY;
}
//---------------------------------------------------------------
HRESULT __stdcall CFactory::LockServer( BOOL bLock )
{
   trace( "CFactory::LockServer(). %d components. %d serverLocks\n", g_cComponents, g_cServerLocks);
   if( bLock )
      InterlockedIncrement( &g_cServerLocks );
   else
      InterlockedDecrement( &g_cServerLocks );
   return S_OK;
}
//---------------------------------------------------------------    CFactory

