// Device.cpp: implementation of the DeviceDriver class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "DeviceDriver.h"
#include <string>
#include <tchar.h>

/**
* @brief class DeviceDriver Constructor.
*/
DeviceDriver::DeviceDriver()
{
  m_hLib = NULL;
}

/**
* @brief class DeviceDriver Destructor.
*/
DeviceDriver::~DeviceDriver()
{
  Close();
}

/**
* @brief Function Close unloads driver dll if necessary.
*/
void DeviceDriver::Close()
{
  if (m_hLib != NULL)
  {
    if (PreUnload2)
    {
      PreUnload2();
    }
    ::FreeLibrary(m_hLib);
    m_hLib = NULL;
  }
}

/**
* @brief Function CheckEntry returns the given Lib function.
*
* @param szEntry - the Lib function name.
* @return void* the Lib function pointer and NULL if it is not present.
*/
void* DeviceDriver::CheckEntry(const char* szEntry)
{
  void* pEntry = ::GetProcAddress(m_hLib, szEntry);
  return pEntry;
}


/**
* @brief Function LoadDevice loads the driver dll and sanity checks
* whether it is actually a driver dll that is loaded.
*
* @param szLibName the full dll path of the driver to be loaded.
* @return bool that indicates whether the dll is loaded and is indeed a driver dll.
*/
bool DeviceDriver::LoadDevice(const char* szLibName)
{
  Close();

  m_hLib = ::LoadLibraryEx(szLibName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
  if (!m_hLib)
  {
    return false;
  }

  // ensure current directory is set to the driver directory so that loading of e.g. ptz_protocols.dll will work - also on older drivers
  std::string strDllName(szLibName);
  // get dir name:
  auto last_backslash = strDllName.rfind("\\");
  std::string dirname = strDllName.substr(0, last_backslash);
  SetCurrentDirectory(dirname.c_str());

  (FARPROC&) GetInfo                        = (FARPROC)CheckEntry(_T("GetInfo"));
  (FARPROC&) GetInfo2                       = (FARPROC)CheckEntry(_T("GetInfo2"));
  (FARPROC&) QueryDeviceTypeStr             = (FARPROC)CheckEntry(_T("QueryDeviceTypeStr"));
  (FARPROC&) PreUnload                      = (FARPROC)CheckEntry(_T("PreUnload"));
  (FARPROC&) PreUnload2                     = (FARPROC)CheckEntry(_T("PreUnload2"));
  (FARPROC&) PostLoad                       = (FARPROC)CheckEntry(_T("PostLoad"));

  //some required functions (simple sanity check)
  if(QueryDeviceTypeStr || GetInfo)
  {
    if (PostLoad)
    {
      PostLoad();
    }
    return true;
  }
  return false;
}
