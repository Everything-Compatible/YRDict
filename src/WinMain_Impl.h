#pragma once
#include "ImplBase.h"
#include "VanillaImpl.h"
#include "InitBase.h"


namespace WinMain_Impl
{
	void UninitLast_CloseLog();
	UNINIT_AFTER_EVERY_GAMEMD_FUNCTION(UninitLast_CloseLog)

	//Replace the original WinMain call so as to insert our own initialization code
	int __stdcall WinMain_Aux(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	FUNCTION_CALL_AUX(0x7CD8EA, WinMain_Aux)


	inline const char* GameInstanceMutex_UUID = "29e3bb2a-2f36-11d3-a72c-0090272fa661";

	/*
	* 
	* As an example of partial implementation
	* 
	*/
	int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	PENDING_FUNCTION_IMPLEMENT(0x6BB9A0, WinMain)
}