#pragma once
#include <Windows.h>
#include <string>
#include <format>

namespace Debug
{
	void Log(const char* pFormat, ...);
	void LogAndMessage(const char* pFormat, ...);
	void LogString(const char* pStr);
	void LogString(const std::string& Str);
	void MessageString(const char* pStr);
	void MessageString(const std::string& Str);
	void LogAndMessageString(const char* pStr);
	void LogAndMessageString(const std::string& Str);

	void LogFormat(const std::string& fmt, auto&&... args) {
		std::string msg = std::format(fmt, std::forward<decltype(args)>(args)...);
		LogString(msg);
	}
	void LogVFormat(const std::string& fmt, std::format_args args) {
		std::string msg = std::vformat(fmt, args);
		LogString(msg);
	}
	void MessageFormat(const std::string& fmt, auto&&... args) {
		std::string msg = std::format(fmt, std::forward<decltype(args)>(args)...);
		MessageString(msg);
	}
	void MessageVFormat(const std::string& fmt, std::format_args args) {
		std::string msg = std::vformat(fmt, args);
		MessageString(msg);
	}
	void LogFormatAndMessage(const std::string& fmt, auto&&... args) {
		std::string msg = std::format(fmt, std::forward<decltype(args)>(args)...);
		LogAndMessageString(msg);
	}
	void LogVFormatAndMessage(const std::string& fmt, std::format_args args) {
		std::string msg = std::vformat(fmt, args);
		LogAndMessageString(msg);
	}
}