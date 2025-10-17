#include "WinMain_Impl.h"
#include "Debug_Impl.h"
#include <SystemData.h>
#include <CRT.h>
#include <Unsorted.h>
#include <Debug.h>
#include "Launch_Impl.h"

namespace WinMain_Impl
{
	void UninitLast_CloseLog()
	{
		Debug_Impl::CloseLogFile("YRDict : Closing Log File at exit...\n");
	}


	int __stdcall WinMain_Aux(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			if (!Debug_Impl::IsLogFileOpen())
				Debug_Impl::OpenLogFile();
		}
		int Result = System::WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			Debug_Impl::FlushLogFile();
		}
		return Result;
	}

	
	int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		//Initialize system params
		Game::hInstance = hInstance;
		Game::MainThreadId = ::GetCurrentThreadId();

		//Show that WW is genius
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			//WW is so genius that we cannot understand the idea behind these numbers
			Debug::Log("%d-%d\n", 1660, 152);
			Debug::Log("%d-%d\n", 1661, 76);
			Debug::Log("%d-%d\n", 1662, 76);
			Debug::Log("%d-%d\n", 1663, 19);
			Debug::Log("%d-%d\n", 1664, 3608);
			Debug::Log("%d-%d\n", 1665, 1752);
			Debug::Log("%d-%d\n", 1666, 3600);
			Debug::Log("%d-%d\n", 1667, 272);
			Debug::Log("%d-%d\n", 1668, 456);
			Debug::Log("%d-%d\n", 1669, 888);
			Debug::Log("%d-%d\n", 1670, 120);
			Debug::Log("%d-%d\n", 1671, 1824);
			Debug::Log("%d-%d\n", 1672, 6040);
			Debug::Log("%d-%d\n", 1673, 352);
			Debug::Log("%d-%d\n", 1674, 760);
			Debug::Log("%d-%d\n", 1675, 328);
			Debug::Log("%d-%d\n", 1676, 116);
			Debug::Log("%d-%d\n", 1677, 90296);
			Debug::Log("%d-%d\n", 1678, 432);
			Debug::Log("%d-%d\n", 1679, 1776);
			Debug::Log("%d-%d\n", 1680, 3792);
			Debug::Log("%d-%d\n", 1681, 24);
			Debug::Log("%d-%d\n", 1682, 21868);
			Debug::Log("%d-%d\n", 1683, 3608);
			Debug::Log("%d-%d\n", 1684, 176);
			Debug::Log("%d-%d\n", 1685, 700);
			Debug::Log("%d-%d\n", 1686, 176);
			Debug::Log("%d-%d\n", 1687, 676);
			Debug::Log("%d-%d\n", 1688, 212);
			Debug::Log("%d-%d\n", 1689, 160);
			Debug::Log("%d-%d\n", 1690, 248);
			Debug::Log("%d-%d\n", 1691, 224);
			Debug::Log("%d-%d\n", 1692, 700);
			Debug::Log("%d-%d\n", 1693, 72);
			Debug::Log("%d-%d\n", 1694, 180);
			Debug::Log("%d-%d\n", 1695, 164);
			Debug::Log("%d-%d\n", 1696, 56);
			Debug::Log("%d-%d\n", 1697, 2280);
			Debug::Log("%d-%d\n", 1698, 3704);
			Debug::Log("%d-%d\n", 1699, 14144);
			Debug::Log("%d-%d\n", 1700, 256);
			Debug::Log("%d-%d\n", 1701, 128);
			Debug::Log("%d-%d\n", 1702, 432);
			Debug::Log("%d-%d\n", 1703, 6336);
			Debug::Log("%d-%d\n", 1704, 1024);
			Debug::Log("%d-%d\n", 1705, 1);
			Debug::Log("%d-%d\n", 1706, 1312);
			Debug::Log("%d-%d\n", 1707, 240);
			Debug::Log("%d-%d\n", 1708, 212);
			Debug::Log("%d-%d\n", 1709, 8);
			Debug::Log("%d-%d\n", 1710, 28);
			Debug::Log("%d-%d\n", 1711, 4);
			Debug::Log("%d-%d\n", 1712, 8);
			Debug::Log("%d-%d\n", 1713, 16);
			Debug::Log("%d-%d\n", 1714, 32);
			Debug::Log("%d-%d\n", 1715, 24);
		}

		// return true if Launcher is found
		if (!CopyProtection_Impl::IsLauncherRunning())
		{
			// CopyProtection -- Exit if Launcher isn't found
			Debug::LogString("Launcher not running....Bail!\n");
			return 0;
		}

		//Ensure that there is only 1 Instance
		//including TS/FS/RA2/YR
		Game::GameInstanceMutex = CreateMutexA(0, FALSE, GameInstanceMutex_UUID);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			HWND hWnd_FromMutex = FindWindowA(GameInstanceMutex_UUID, NULL);
			if (hWnd_FromMutex)
			{
				SetForegroundWindow(hWnd_FromMutex);
				ShowWindow(hWnd_FromMutex, SW_RESTORE);
			}
			if (Game::GameInstanceMutex)
			{
				CloseHandle(Game::GameInstanceMutex);
				Game::GameInstanceMutex = NULL;
			}
			Debug::LogString("TibSun is already running...Bail!\n");
			// Exit -- Game is already running
			return 0;                                  
		}
		// Ensure that only one game instance is running
		Debug::LogString("Create AppMutex okay.\n");                    

		//To be Implemented
	}
}