//
// Registry.cpp
//

#include <objbase.h>
#include <cassert>
#include <tchar.h>

#include "registry.h"

////////////////////////////////////////////////////////
//
// Internal helper functions prototypes
//

// Set the given key and its value.
BOOL setKeyAndValue( const TCHAR* pszPath,
                     const TCHAR* szSubkey,
                     const TCHAR* szValue );

// Convert a CLSID into a TCHAR string.
void CLSIDtotchar( const CLSID& clsid,
                  TCHAR* szCLSID,
                  int length );

// Delete szKeyChild and all of its descendents.
LONG recursiveDeleteKey( HKEY hKeyParent, const TCHAR* szKeyChild );

////////////////////////////////////////////////////////
//
// Constants
//

// Size of a CLSID as a string
const int CLSID_STRING_SIZE = 39;

/////////////////////////////////////////////////////////
//
// Public function implementation
//

//
// Register the component in the registry.
//
HRESULT RegisterServer( HMODULE hModule,            // DLL module handle
                        const CLSID& clsid,         // Class ID
                        const TCHAR* szFriendlyName, // Friendly Name
                        const TCHAR* szVerIndProgID, // Programmatic
                        const TCHAR* szProgID )       //   IDs
{
   // Get server location.
   TCHAR szModule[512];
   DWORD dwResult =
      ::GetModuleFileName( hModule,
                           szModule,
                           sizeof( szModule ) / sizeof( TCHAR ) );
   assert( dwResult != 0 );

   // Convert the CLSID into a TCHAR.
   TCHAR szCLSID[CLSID_STRING_SIZE];
   CLSIDtotchar( clsid, szCLSID, sizeof( szCLSID ) );

   // Build the key CLSID\\{...}
   TCHAR szKey[64];
   _tcscpy_s( szKey, 64, TEXT( "CLSID\\" ) );
   _tcscat_s( szKey, 64, szCLSID );

   // Add the CLSID to the registry.
   setKeyAndValue( szKey, NULL, szFriendlyName );

   // Add the server filename subkey under the CLSID key.
   setKeyAndValue( szKey, TEXT( "InprocServer32" ), szModule );

   // Add the ProgID subkey under the CLSID key.
   setKeyAndValue( szKey, TEXT( "ProgID" ), szProgID );

   // Add the version-independent ProgID subkey under CLSID key.
   setKeyAndValue( szKey, TEXT( "VersionIndependentProgID" ),
                   szVerIndProgID );

   // Add the version-independent ProgID subkey under HKEY_CLASSES_ROOT.
   setKeyAndValue( szVerIndProgID, NULL, szFriendlyName );
   setKeyAndValue( szVerIndProgID, TEXT( "CLSID" ), szCLSID );
   setKeyAndValue( szVerIndProgID, TEXT( "CurVer" ), szProgID );

   // Add the versioned ProgID subkey under HKEY_CLASSES_ROOT.
   setKeyAndValue( szProgID, NULL, szFriendlyName );
   setKeyAndValue( szProgID, TEXT( "CLSID" ), szCLSID );

   return S_OK;
}

//
// Remove the component from the registry.
//
LONG UnregisterServer( const CLSID& clsid,         // Class ID
                       const TCHAR* szVerIndProgID, // Programmatic
                       const TCHAR* szProgID )       //   IDs
{
   // Convert the CLSID into a TCHAR.
   TCHAR szCLSID[CLSID_STRING_SIZE];
   CLSIDtotchar( clsid, szCLSID, sizeof( szCLSID ) );

   // Build the key CLSID\\{...}
   TCHAR szKey[64];
   _tcscpy_s( szKey, 64, TEXT( "CLSID\\" ) );
   _tcscat_s( szKey, 64, szCLSID );

   // Delete the CLSID Key - CLSID\{...}
   LONG lResult = recursiveDeleteKey( HKEY_CLASSES_ROOT, szKey );
   assert( ( lResult == ERROR_SUCCESS ) ||
      ( lResult == ERROR_FILE_NOT_FOUND ) ); // Subkey may not exist.

// Delete the version-independent ProgID Key.
   lResult = recursiveDeleteKey( HKEY_CLASSES_ROOT, szVerIndProgID );
   assert( ( lResult == ERROR_SUCCESS ) ||
      ( lResult == ERROR_FILE_NOT_FOUND ) ); // Subkey may not exist.

// Delete the ProgID key.
   lResult = recursiveDeleteKey( HKEY_CLASSES_ROOT, szProgID );
   assert( ( lResult == ERROR_SUCCESS ) ||
      ( lResult == ERROR_FILE_NOT_FOUND ) ); // Subkey may not exist.

   return S_OK;
}

///////////////////////////////////////////////////////////
//
// Internal helper functions
//

// Convert a CLSID to a TCHAR string.
void CLSIDtotchar( const CLSID& clsid,
                  TCHAR* szCLSID,
                  int length )
{
   assert( length >= CLSID_STRING_SIZE );
   // Get CLSID
   LPOLESTR wszCLSID = NULL;
   HRESULT hr = StringFromCLSID( clsid, &wszCLSID );
   assert( SUCCEEDED( hr ) );

#ifndef UNICODE
   // Covert from wide characters to non-wide.
   wcstombs( szCLSID, wszCLSID, length );
#else
   _tcscpy_s( szCLSID, CLSID_STRING_SIZE, wszCLSID );
#endif // !UNICODE


   // Free memory.
   CoTaskMemFree( wszCLSID );
}

//
// Delete a key and all of its descendents.
//
LONG recursiveDeleteKey( HKEY hKeyParent,           // Parent of key to delete
                         const TCHAR* lpszKeyChild )  // Key to delete
{
   // Open the child.
   HKEY hKeyChild;
   LONG lRes = RegOpenKeyEx( hKeyParent,
                             lpszKeyChild,
                             0,
                             KEY_ALL_ACCESS,
                             &hKeyChild );
   if ( lRes != ERROR_SUCCESS )
   {
      return lRes;
   }

   // Enumerate all of the decendents of this child.
   FILETIME time;
   TCHAR szBuffer[256];
   DWORD dwSize = 256;
   while ( RegEnumKeyEx( hKeyChild, 0, szBuffer, &dwSize, NULL, NULL, NULL, &time ) == S_OK )
   {
      // Delete the decendents of this child.
      lRes = recursiveDeleteKey( hKeyChild, szBuffer );
      if ( lRes != ERROR_SUCCESS )
      {
         // Cleanup before exiting.
         RegCloseKey( hKeyChild );
         return lRes;
      }
      dwSize = 256;
   }

   // Close the child.
   RegCloseKey( hKeyChild );

   // Delete this child.
   return RegDeleteKey( hKeyParent, lpszKeyChild );
}

//
// Create a key and set its value.
//   - This helper function was borrowed and modifed from
//     Kraig Brockschmidt's book Inside OLE.
//
BOOL setKeyAndValue( const TCHAR* szKey,
                     const TCHAR* szSubkey,
                     const TCHAR* szValue )
{
   HKEY hKey;
   TCHAR szKeyBuf[1024];

   // Copy keyname into buffer.
   _tcscpy_s( szKeyBuf, 1024, szKey );

   // Add subkey name to buffer.
   if ( szSubkey != NULL )
   {
      _tcscat_s( szKeyBuf, 1024, TEXT( "\\" ) );
      _tcscat_s( szKeyBuf, 1024, szSubkey );
   }

   // Create and open key and subkey.
   long lResult = RegCreateKeyEx( HKEY_CLASSES_ROOT,
                                  szKeyBuf,
                                  0,
                                  NULL,
                                  REG_OPTION_NON_VOLATILE,
                                  KEY_ALL_ACCESS,
                                  NULL,
                                  &hKey,
                                  NULL );
   if ( lResult != ERROR_SUCCESS )
   {
      return FALSE;
   }

   // Set the Value.
   if ( szValue != NULL )
   {
      //;
      RegSetValueEx( hKey, NULL, 0, REG_SZ, ( BYTE * )szValue, sizeof( TCHAR )*( _tcslen( szValue ) + 1 ) );
   }

   RegCloseKey( hKey );
   return TRUE;
}
