#include "Debug.h"
#include <CRT.h>
#include <MessageListClass.h>
#include "../src/VanillaImpl.h"
#include "../src/Debug_Impl.h"

namespace Debug
{
	void Log(const char* pFormat, ...)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			va_list args;
			va_start(args, pFormat);
			Debug_Impl::LogWithVArgs(pFormat, args);
			va_end(args);
		}
		else
		{
			JMP_STD(0x4068E0);
		}
	}
	void LogAndMessage(const char* pFormat, ...)
	{
		va_list args;
		va_start(args, pFormat);
		char buffer[0x1000];
		vsnprintf(buffer, 0x1000, pFormat,args);
		va_end(args);
		LogString(buffer);
		MessageString(buffer);
	}
	void LogString(const char* pStr)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			Debug_Impl::LogString(pStr);
		}
		else
		{
			Log("%s", pStr);
		}
	}
	void LogString(const std::string& Str)
	{
		if constexpr (!UseVanillaImpl_Supplementary)
		{
			Debug_Impl::LogString(Str.c_str());
		}
		else
		{
			Log("%s", Str.c_str());
		}
	}
	void MessageString(const char* pStr)
	{
		wchar_t buffer[0x1000];
		CRT::mbstowcs(buffer, pStr, 0x1000);
		MessageListClass::Instance.PrintMessage(buffer);
	}
	void MessageString(const std::string& Str)
	{
		wchar_t buffer[0x1000];
		CRT::mbstowcs(buffer, Str.c_str(), 0x1000);
		MessageListClass::Instance.PrintMessage(buffer);
	}
	void LogAndMessageString(const char* pStr)
	{
		LogString(pStr);
		MessageString(pStr);
	}
	void LogAndMessageString(const std::string& Str)
	{
		LogString(Str);
		MessageString(Str);
	}
}