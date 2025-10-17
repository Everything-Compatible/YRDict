#pragma once
#include <cstdio>
#include <filesystem>
#include "ImplBase.h"
#include "VanillaImpl.h"

namespace Debug_Impl
{
	extern FILE* LogFile;
	extern FILE* TempLogFile;
	extern std::filesystem::path LogFilePath;
	extern std::filesystem::path TempLogFilePath;

	bool IsLogFileOpen();
	void OpenLogFile();

	//nullptr if no CloseState is needed
	void CloseLogFile(const char* CloseState);
	void FlushLogFile();
	void LogWithVArgsUnflushed(const char* pFormat, va_list args);
	void LogUnflushed(const char* Str);

	void __cdecl Log(const char* pFormat, ...);
	FUNCTION_IMPLEMENT(0x4A4AC0, Log);
	FUNCTION_IMPLEMENT(0x4068E0, Log);

	void LogWithVArgs(const char* pFormat, va_list args);
	void LogString(const char* Str);
}