#pragma once
#include "ImplBase.h"
#include "VanillaImpl.h"
#include <Debug.h>

namespace CopyProtection_Impl
{
	inline const char* MutexGUID = "48BC11BD-C4D7-466b-8A31-C6ABBAD47B3E";
	inline const char* NotifySignalGUID = "D6E7FC97-64F9-4d28-B52C-754EDF721C6F";
	inline const char* Launcher_RegKey = "WChat\\SysID";
	inline const UINT Launcher_HereIAm_Message = 0xBEEFu;

	bool IsLauncherRunning();
	FUNCTION_IMPLEMENT(0x49F5C0, IsLauncherRunning);

	bool NotifyLauncher();
	FUNCTION_IMPLEMENT(0x49F620, NotifyLauncher);

	bool CheckVersion();
	FUNCTION_IMPLEMENT(0x49F920, CheckVersion);

	bool CheckProtectedData();
	FUNCTION_IMPLEMENT(0x49F7A0, CheckProtectedData);

	void __fastcall DispatchLauncherMessage(MSG* pMsg);
	FUNCTION_IMPLEMENT(0x49F740, DispatchLauncherMessage);

	LSTATUS ShutDown();
	FUNCTION_IMPLEMENT(0x49F8B0, ShutDown);

	inline const char* mProtectData_Failed = "EEK!";
	inline const char* mProtectData_Success = "UIDATA,3DDATA,MAPS";
	DEFINE_REFERENCE(char*, _mProtectedData, 0x89F75C)
};

namespace InitCheck_Impl
{
	DWORD GetFreeDiskSpaceKB();
	FUNCTION_IMPLEMENT(0x48DD50, GetFreeDiskSpaceKB);
}