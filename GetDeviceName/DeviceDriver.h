
#pragma once

/* === String to hold the proxy server application name =============== */
#include <Windows.h>
#include <string>
extern std::string m_strAppName;

/* === END String to hold the proxy server application name =========== */


/* === Class DeviceDriver ============================================= */

/**
* @brief class DeviceDriver to hold the driver dll handle and all the 
* Lib functions available.
*/
class DeviceDriver  
{
public:
  /** @name Constructors and destructors */ //@{
	DeviceDriver();
	virtual ~DeviceDriver();
  //@}

// Attributes
public:
  HINSTANCE       m_hLib;

public:
  /** @name Driver load/unload functions */ //@{
  bool            LoadDevice(const char* szLibName);
  void            Close();
  //@}

  /** @name Utility functions */ //@{
  void* CheckEntry(const char* szEntry);
  //@}

  /** @name Lib functions */ //@{
  DWORD           (PASCAL *GetInfo)           (LPCSTR, LPCSTR, char*, char*);
  DWORD           (PASCAL *GetInfo2)           (LPCSTR, LPCSTR, LPCSTR, char*, char*, char*);
  void            (PASCAL *QueryDeviceTypeStr)(LPTSTR);
  BOOL            (PASCAL *PreUnload)(VOID);
  void            (PASCAL *PreUnload2)(VOID);
  void            (PASCAL *PostLoad)(VOID);

  //@}
};

/* === END Class DeviceDriver ========================================== */
