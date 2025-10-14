#pragma once
#include "ImplBase.h"
#include <CCFileClass.h>
#include <CRT.h>

/*
FileClass

Full Implementation
*/
DEFINE_VIRTUAL_IMPL(FileClass, 0x7F08BCu)
{
public:

	void DTOR() { reset_vtable(); }
	MAKE_DTOR;

	bool SetFileTime(DWORD FileTime) { return 0; }

	DWORD GetFileTime() { return 0; }

	void* ReadWholeFile()
	{
		void* FileBuffer = nullptr;
		int szFileSize;

		if (This->Exists(false))   
		{
			szFileSize = This->GetFileSize();  
			FileBuffer = YRMemory::Allocate(szFileSize);
			if (FileBuffer)
				This->ReadBytes(FileBuffer, szFileSize);
		}
		return FileBuffer;
	}

	//Other Functions are all not implemented

	DEFINE_IMPLEMENT
	{
		IMPLEMENT(0x65C610u, DynamicDTOR)
		IMPLEMENT(0x65C5F0u, SetFileTime)
		IMPLEMENT(0x65C600u, GetFileTime)
		IMPLEMENT(0x4A3890u, ReadWholeFile)
	}
};
DEFINE_IMPL_END(FileClass)


/*
RawFileClass

Full Implementation
*/
DEFINE_VIRTUAL_IMPL(RawFileClass, 0x7F0904u)
{
public:
	void DTOR();
	MAKE_DTOR;
	
	const char* GetFileName() const { return This->FileName; }
	const char* SetFileName(const char* pFileName);

	void CDCheck(DWORD ErrorCode, bool bUnk, char* FileName) {}

	DEFINE_IMPLEMENT
	{
		IMPLEMENT(0x65CA00u, StaticDTOR)
		IMPLEMENT(0x65D3A0u, DynamicDTOR)
		IMPLEMENT(0x401940u, GetFileName)
		IMPLEMENT(0x65CAC0u, SetFileName)
		IMPLEMENT(0x65CA70u, CDCheck)
	}
};
DEFINE_IMPL_END(RawFileClass)
