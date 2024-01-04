#include <iostream>
#include <fstream>
#include <regex>
#include "DeviceDriver.h"

#define FROM_GET_INFO "FromGetInfo"
#define FROM_QUERY_DEVICE_TYPE_STR "FromQueryDeviceTypeStr"

int main(int argc, char* argv[])
{
  
  if (argc < 4)
  {
    std::cout << "Invalid number of arguments! Expected: <GetDeviceNameOption> <DeviceIP> <user:pass> <DriverDllPath>" << std::endl
              << "\t- <GetDeviceNameOptions>: " << FROM_GET_INFO << " or " << FROM_QUERY_DEVICE_TYPE_STR << std::endl;
    return 1;
  }

  std::string strGetDeviceNameOption = argv[1];
  std::string strDeviceIP = argv[2];
  std::string strUserPass = argv[3];
  std::string strDriverDllPath = argv[4];

  std::cout << "Loaded with args " << strGetDeviceNameOption << " " << strDeviceIP << " " << strUserPass << " " << strDriverDllPath << std::endl;
  if (strGetDeviceNameOption != FROM_GET_INFO && strGetDeviceNameOption != FROM_QUERY_DEVICE_TYPE_STR)
  {
    std::cout << "Invalid GetDeviceName option: " << strGetDeviceNameOption << ". Expected " << FROM_GET_INFO << " or " << FROM_QUERY_DEVICE_TYPE_STR << std::endl;
    return 1;
  }
 
  // strip schema from ip:
  std::regex schema("https?://");
  strDeviceIP = std::regex_replace(strDeviceIP, schema, "");  

  DeviceDriver d;
  if (!d.LoadDevice(strDriverDllPath.c_str()))
  {
    std::cout << "Failed to load driver " << strDriverDllPath << std::endl;
    return 1;
  }

  // the result we're looking for
  char szDeviceName[256] = { 0 };

  if (strGetDeviceNameOption == FROM_GET_INFO)
  {
    char sMacAddr[256] = { 0 };
    d.GetInfo(strDeviceIP.c_str(), strUserPass.c_str(), sMacAddr, szDeviceName);
  }
  else 
  {
    d.QueryDeviceTypeStr(szDeviceName);
  }

  // write the output: 
  char szModuleName[256] = { 0 };
  GetModuleFileName(NULL, szModuleName, 256);
  std::string strModuleName(szModuleName);
  std::string strOutputFile = strModuleName.substr(0, strModuleName.rfind("\\")) + "\\output.txt";

  std::ofstream output;
  output.open(strOutputFile, std::ios::trunc);
  output << szDeviceName;
  output.close();

  d.Close();
  return 0;
}
