#pragma once
#include <CRT.h>
#include <DynamicPatch.h>
#include <vector>

/*
辅助变量，不直接使用
Helper Variable, NO Direct Usage.
*/
inline std::vector<void(*)()> __InitBeforeEveryGameMDFunctions{};
inline std::vector<void(*)()> __UninitAfterEveryGameMDFunctions{};

/*
辅助类型，不直接使用
Helper Class, NO Direct Usage.
*/
inline void __cdecl UninitAfterEveryGameMDFunction()
{
	for (auto& func : __UninitAfterEveryGameMDFunctions)func();
}
inline void InitBeforeEveryGameMDFunction()
{
	for (auto& func : __InitBeforeEveryGameMDFunctions)func();
	CRT::atexit(UninitAfterEveryGameMDFunction);
}
inline void InitBeforeEveryGameMDFunction_Register()
{
	CodeModifier::ModifyPointer(0x812000, InitBeforeEveryGameMDFunction);
}
struct InitBeforeEveryGameMDFunctionHelper
{
	InitBeforeEveryGameMDFunctionHelper() = delete;
	InitBeforeEveryGameMDFunctionHelper(void(*func)())
	{
		__InitBeforeEveryGameMDFunctions.push_back(func);
	}
	~InitBeforeEveryGameMDFunctionHelper() = default;
};
struct UninitAfterEveryGameMDFunctionHelper
{
	UninitAfterEveryGameMDFunctionHelper() = delete;
	UninitAfterEveryGameMDFunctionHelper(void(*func)())
	{
		__UninitAfterEveryGameMDFunctions.push_back(func);
	}
	~UninitAfterEveryGameMDFunctionHelper() = default;
};


/*

NOTE:
Call Sequence:

CTOR of global/static objects in YRDict.dll
DllMain(DLL_PROCESS_ATTACH) of YRDict.dll
EntryPoint of GameMD.exe
InitBeforeEveryGameMDFunction
CTOR of global/static objects in GameMD.exe
WinMain of GameMD.exe
...
Exit of GameMD.exe
DTOR of global/static objects in GameMD.exe
UninitAfterEveryGameMDFunction
ExitProcess

*/
/*

在每次GameMD.exe启动前调用的函数
与此同时，在GameMD.exe退出后调用对应的反初始化函数
Functions to be called before every GameMD.exe launch
and corresponding uninitialization functions after GameMD.exe exit.

*/
#define INIT_BEFORE_EVERY_GAMEMD_FUNCTION(func) \
	inline InitBeforeEveryGameMDFunctionHelper __InitBeforeEveryGameMDFunctionHelper_##func(func);
#define UNINIT_AFTER_EVERY_GAMEMD_FUNCTION(func) \
	inline UninitAfterEveryGameMDFunctionHelper __UninitAfterEveryGameMDFunctionHelper_##func(func);
