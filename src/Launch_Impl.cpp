#include "Launch_Impl.h"
#include "Windows.h"
#include <VersionClass.h>
#include <Unsorted.h>

namespace CopyProtection_Impl
{

    bool IsLauncherRunning()
    {
        if constexpr (UseVanillaImpl_Critical)
        {
            HANDLE hMutex;

            Debug::LogString("COPYPROTECTION - Checking if launcher is running\n");

            hMutex = CreateMutexA(0, 0, MutexGUID);
            bool HasLauncher = (GetLastError() == ERROR_ALREADY_EXISTS);
            if (hMutex)CloseHandle(hMutex);

            Debug::Log("result was %d\n", HasLauncher);
            return HasLauncher;
        }
        else
        {
            return true;
        }
    }

    bool NotifyLauncher()
    {
        if constexpr (UseVanillaImpl_Critical)
        {
            HANDLE hEvent;
            DWORD NextCheckTime;
            MSG Msg;

            const DWORD MillisecondsOf1Minute = 60000;
            const DWORD MillisecondsOf10Seconds = 10000;
            
            Debug::LogString("COPYPROTECTION - Notify launcher\n");

            PeekMessageA(&Msg, 0, WM_USER, WM_USER, PM_NOREMOVE);
            DWORD NotifyWaitEndTime = timeGetTime() + MillisecondsOf1Minute;

            if (timeGetTime() < NotifyWaitEndTime)
            {

                while (1)
                {
                    hEvent = OpenEventA(EVENT_MODIFY_STATE, TRUE, NotifySignalGUID);
                    if (hEvent)break;
                    Sleep(0);
                    if (timeGetTime() >= NotifyWaitEndTime)
                    {
                        Debug::LogString("***** Failed to notify launcher!\n");
                        return false;
                    }
                }

                SetEvent(hEvent);
                CloseHandle(hEvent);
                Debug::LogString("Launcher notified.\n");
                Debug::LogString("Waiting for message from launcher.\n");
                NextCheckTime = timeGetTime() + MillisecondsOf10Seconds;
                if (timeGetTime() <= NextCheckTime)
                {
                    while (!PeekMessageA(
                        &Msg, 0,
                        Launcher_HereIAm_Message,
                        Launcher_HereIAm_Message,
                        PM_REMOVE) ||
                        Msg.message != Launcher_HereIAm_Message)
                    {
                        Sleep(0);
                        if (timeGetTime() > NextCheckTime)
                        {
                            Debug::LogString("***** Failed to notify launcher!\n");
                            return false;
                        }
                    }
                    _mProtectedData = (char*)MapViewOfFileEx((HANDLE)Msg.lParam, FILE_MAP_ALL_ACCESS, 0, 0, 0, 0);
                    if (_mProtectedData)
                        return true;
                }
            }
            Debug::LogString("***** Failed to notify launcher!\n");
            return false;
		}
        else
        {
            return true;
        }
    }

    bool CheckVersion()
    {
        if constexpr (UseVanillaImpl_Critical)
        {
            LSTATUS RegQueryResult;
            HKEY hKey;
            DWORD RegKeycbData;
            DWORD RegKeyType;
            DWORD VersionFromRegister;

            if (RegOpenKeyExA(HKEY_CLASSES_ROOT, Launcher_RegKey, 0, KEY_READ, &hKey))
                return 0;
            RegKeycbData = 4;
            RegQueryResult = RegQueryValueExA(hKey, "ID", 0, &RegKeyType, (LPBYTE)&VersionFromRegister, &RegKeycbData);
            RegCloseKey(hKey);
            if (RegQueryResult || RegKeyType != REG_DWORD)
                return 0;
            return VersionFromRegister == VersionClass::Instance.Version_Number();
        }
        else
        {
            return true;
        }
    }

    bool CheckProtectedData()
    {
        if constexpr (UseVanillaImpl_Critical)
        {
            const char* ProtectedStr;

            Debug::LogString("COPYPROTECTION - Validating\n");

            ProtectedStr = _mProtectedData;
            if (!_mProtectedData)
                ProtectedStr = mProtectData_Failed;
            Debug::Log("_mProtectedData = %d (%s)\n", _mProtectedData, ProtectedStr);
            if (_mProtectedData)
                return strcmp(_mProtectedData, mProtectData_Success) == 0;

            //Refolded Inline Function Calls
            return CheckVersion();
        }
        else
        {
            return true;
        }
    }

    void __fastcall DispatchLauncherMessage(MSG* pMsg)
    {
        if constexpr (UseVanillaImpl_Critical)
        {
            if (pMsg->message == Launcher_HereIAm_Message)
            {
                Debug::LogString("COPYPROTECTION - Received message from launcher.\n");
                _mProtectedData = (char*)MapViewOfFileEx((HANDLE)pMsg->lParam, FILE_MAP_ALL_ACCESS, 0, 0, 0, 0);
                if (_mProtectedData)
                    Debug::Log("The message says: %s\n", _mProtectedData);
                else
                    Debug::LogString("***** MapViewOfFileEx() Failed!\n");
            }
        }
        else return;
    }

    LSTATUS ShutDown()
    {
        if constexpr (UseVanillaImpl_Critical)
        {
            LSTATUS RegOpenResult;
            HKEY phkResult;

            Debug::LogString("COPYPROTECTION - Shutdown\n");
            if (_mProtectedData)
            {
                UnmapViewOfFile(_mProtectedData);
                _mProtectedData = nullptr;
            }
            RegOpenResult = RegOpenKeyExA(HKEY_CLASSES_ROOT, Launcher_RegKey, 0, KEY_ALL_ACCESS, &phkResult);
            if (!RegOpenResult)
            {
                RegDeleteValueA(phkResult, "ID");
                RegOpenResult = RegCloseKey(phkResult);
            }
            return RegOpenResult;
        }
        else
        {
            return ERROR_SUCCESS;
        }
    }
}


namespace InitCheck_Impl
{
    DWORD GetFreeDiskSpaceKB()
    {
        HMODULE hKernal32; 
        BOOL(__stdcall * GetDiskFreeSpaceExA)(LPCSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER); 
        unsigned int FreeDiskSpaceKB; 
        DWORD SectorsPerCluster; 
        DWORD BytesPerSector; 
        DWORD NumberOfFreeClusters; 
        DWORD TotalNumberOfClusters;
        ULARGE_INTEGER FreeBytesAvailableToCaller; 
        ULARGE_INTEGER TotalNumberOfFreeBytes; 
        ULARGE_INTEGER TotalNumberOfBytes; 

        Debug::LogString("Checking available disk space\n");
        hKernal32 = GetModuleHandleA("KERNEL32.DLL");
        if (hKernal32)
        {
            GetDiskFreeSpaceExA = reinterpret_cast<decltype(GetDiskFreeSpaceExA)>
                (GetProcAddress(hKernal32, "GetDiskFreeSpaceExA"));
            if (GetDiskFreeSpaceExA)
            {
                Debug::LogString("Using GetDiskFreeSpaceEx\n");
                if (GetDiskFreeSpaceExA(NULL, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes))
                {
                    FreeDiskSpaceKB = Game::F2I64((FreeBytesAvailableToCaller.HighPart * 4294967296.0 + FreeBytesAvailableToCaller.LowPart) * 0.0009765625);
                    Debug::Log("Free disk space is %d Mb\n", FreeDiskSpaceKB >> 10);
                    return FreeDiskSpaceKB;
                }

                //why there is 0 to %s ?!
                Debug::Log("GetDiskFreeSpaceEx failed with error code %d - %s\n", GetLastError(), 0);
            }
            else
            {
                //why there is 0 to %s ?!
                Debug::Log("GetProcAddress failed with error code %d - %s\n", GetLastError(), 0);
            }
        }
        else
        {
            Debug::LogString("Failed to get module handle for KERNEL32.DLL\n");
        }
        if (!GetDiskFreeSpaceA(0, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters))
            return 0;
        FreeDiskSpaceKB = NumberOfFreeClusters * ((BytesPerSector * SectorsPerCluster) >> 10);
        Debug::Log("Free disk space is %d Mb\n", FreeDiskSpaceKB >> 10);
        return FreeDiskSpaceKB;
    }
}