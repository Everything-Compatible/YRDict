#include "Debug_Impl.h"
#include "VanillaImpl.h"
#include <Debug.h>
#include <Unsorted.h>
#include <CRT.h>

namespace Debug_Impl
{
	FILE* LogFile = nullptr;
	FILE* TempLogFile = nullptr;
	std::filesystem::path LogFilePath{};
	std::filesystem::path TempLogFilePath{};

	bool IsLogFileOpen()
	{
		return LogFile != nullptr;
	}

	void MakeLogFile()
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			static bool made = 0;
			if (!made) {
				wchar_t path[MAX_PATH];

				SYSTEMTIME time;

				GetLocalTime(&time);
				GetCurrentDirectoryW(MAX_PATH, path);

				LogFilePath = path;
				LogFilePath /= L"Debug";

				CreateDirectoryW(LogFilePath.c_str(), nullptr);

				wchar_t subpath[64];
				swprintf(subpath, 64, L"debug.%04u%02u%02u-%02u%02u%02u.log",
					time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

				TempLogFilePath = LogFilePath / L"debug.log";
				LogFilePath /= subpath;

				made = 1;
			}
		}
	}

	void OpenLogFile()
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{

			MakeLogFile();
			CloseLogFile(nullptr);

			LogFile = CRT::_fsopen(LogFilePath.string().c_str(), "w", _SH_DENYWR);
			if (!LogFile) {
				wchar_t msg[100] = L"\0";
				LogFile = nullptr;
				wsprintfW(msg, L"Log file failed to open. Error code = %X(%hs)", errno, strerror(errno));
				MessageBoxW(Game::hWnd, LogFilePath.c_str(), msg, MB_OK | MB_ICONEXCLAMATION);
				ExitProcess(1);
			}
			TempLogFile = CRT::_fsopen(TempLogFilePath.string().c_str(), "w", _SH_DENYWR);
			if (!TempLogFile) {
				wchar_t msg[100] = L"\0";
				TempLogFile = nullptr;
				wsprintfW(msg, L"Temp log file failed to open. Error code = %X(%hs)", errno, strerror(errno));
				MessageBoxW(Game::hWnd, TempLogFilePath.c_str(), msg, MB_OK | MB_ICONEXCLAMATION);
				ExitProcess(1);
			}
		}
	}

	void CloseLogFile(const char* CloseState)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			if (IsLogFileOpen())
			{
				if (CloseState != nullptr)
					Debug::LogString(CloseState);
				else
					Debug::LogString("YRDict : Closing Log File...\n");
				CRT::fclose(LogFile);
				CRT::fclose(TempLogFile);
				LogFile = nullptr;
				TempLogFile = nullptr;
			}
		}
	}

	void FlushLogFile()
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			if (IsLogFileOpen())
			{
				CRT::fflush(LogFile);
				CRT::fflush(TempLogFile);
			}
		}
	}

	void LogWithVArgsUnflushed(const char* pFormat, va_list args)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			CRT::vfprintf(LogFile, pFormat, args);
			CRT::vfprintf(TempLogFile, pFormat, args);
		}
	}

	void LogUnflushed(const char* Str)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			CRT::fprintf(LogFile, "%s", Str);
			CRT::fprintf(TempLogFile, "%s", Str);
		}
	}

	void __cdecl Log(const char* pFormat, ...)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			if (!IsLogFileOpen())
				OpenLogFile();

			va_list args;
			va_start(args, pFormat);
			LogWithVArgsUnflushed(pFormat, args);
			va_end(args);

			FlushLogFile();
		}
	}

	void LogWithVArgs(const char* pFormat, va_list args)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			if (!IsLogFileOpen())
				OpenLogFile();

			LogWithVArgsUnflushed(pFormat, args);

			FlushLogFile();
		}
	}

	void LogString(const char* Str)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			if (!IsLogFileOpen())
				OpenLogFile();
			
			LogUnflushed(Str);

			FlushLogFile();
		}
	}
}