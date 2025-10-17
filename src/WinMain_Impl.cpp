#include "WinMain_Impl.h"
#include "Debug_Impl.h"
#include <SystemData.h>
#include <CRT.h>

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
}