#pragma once
#include <objbase.h>
#include <comdef.h>
#include <comip.h>

struct __declspec( uuid( "34441F2B-0E86-423F-B246-7794CC81BAC8" ) ) ISysInfo;
interface ISysInfo : IUnknown
{
   virtual HRESULT __stdcall OSversion( char** pResult ) = 0;
   virtual HRESULT __stdcall MBvendor ( char** pResult ) = 0;
   virtual HRESULT __stdcall CPUcores ( char** pResult ) = 0;
   virtual HRESULT __stdcall monitors ( char** pResult ) = 0;
};
//static const GUID IID_SysInfo ={ 0x34441f2b, 0xe86, 0x423f, { 0xb2, 0x46, 0x77, 0x94, 0xcc, 0x81, 0xba, 0xc8 } };// {34441F2B-0E86-423F-B246-7794CC81BAC8}
static const GUID CLSID_SysInfo_Component ={ 0x68961d0, 0x18c4, 0x4599, { 0xbd, 0xe1, 0xe5, 0xa6, 0xc9, 0xfd, 0x84, 0xd0 } };// {068961D0-18C4-4599-BDE1-E5A6C9FD84D0}
_COM_SMARTPTR_TYPEDEF( ISysInfo, __uuidof( ISysInfo ) );

