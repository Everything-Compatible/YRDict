#pragma once
#include "ImplBase.h"
#include <CCFileClass.h>
#include <CRT.h>

/*
FileClass

Full Implementation
*/
DEFINE_DATA_TYPE(FileClass)
{
	bool SkipCDCheck;
};
DEFINE_VIRTUAL_IMPL(FileClass, 0x7F08BCu)
{
public:
	//Constructor
	FileClass* CTOR() { return This; }

	//Destructor
	void DTOR() { reset_vtable(); }
	MAKE_DTOR;

	//Virtual Function Implements
	bool SetFileTime(DWORD FileTime) { return 0; }
	DWORD GetFileTime() { return 0; }

	//Non-virtual Function Implements
	void* ReadWholeFile();

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
DEFINE_DATA_TYPE(RawFileClass): BASE_DATA(FileClass)
{
	FileAccessMode FileAccess;
	int FilePointer;
	int FileSize;
	HANDLE Handle;
	const char* FileName;
	short unknown_short_1C;
	short unknown_short_1E;
	bool FileNameAllocated;
};
DEFINE_VIRTUAL_IMPL(RawFileClass, 0x7F0904u)
{
public:
	//Constructor
	RawFileClass* CTOR(const char* pFileName);

	//Destructor
	void DTOR();
	MAKE_DTOR;
	
	//Virtual Function Implements
	const char* GetFileName() const;
	const char* SetFileName(const char* pFileName);
	BOOL CreateFile();
	BOOL DeleteFile();
	bool Exists(bool writeShared = false);
	bool HasHandle() const;
	bool Open(FileAccessMode access);
	bool OpenEx(const char* pFileName, FileAccessMode access);
	int ReadBytes(void* pBuffer, int nNumBytes);//return TotalBytesRead
	int Seek(int offset, FileSeekMode seek);
	int GetFileSize();
	int WriteBytes(void* pBuffer, int nNumBytes);
	void Close();
	DWORD GetFileTime();
	bool SetFileTime(DWORD FileTime);
	void CDCheck(DWORD ErrorCode, bool canRetry, const char* FileName) {}

	//Non-virtual Function Implements
	void Bias(int offset = 0, int length = -1);

private:
	//Helper functions
	int DirectSeek(int offset, FileSeekMode seek);
	int IndirectSeek(int offset, FileSeekMode seek);
	int FixSeekPointer(int FixedOffset, FileSeekMode FixedSeek, int DirectSeekResult);
public:
	DEFINE_IMPLEMENT
	{
		IMPLEMENT(0x65CA80u, CTOR);
		IMPLEMENT(0x65CA00u, StaticDTOR)
		IMPLEMENT(0x65D3A0u, DynamicDTOR)
		IMPLEMENT(0x401940u, GetFileName)
		IMPLEMENT(0x65CAC0u, SetFileName)
		IMPLEMENT(0x65D150u, CreateFile)
		IMPLEMENT(0x65D190u, DeleteFile)
		IMPLEMENT(0x65CBF0u, Exists)
		IMPLEMENT(0x65D420u, HasHandle)
		IMPLEMENT(0x65CB50u, Open)
		IMPLEMENT(0x65CB30u, OpenEx)
		IMPLEMENT(0x65CCE0u, ReadBytes)
		IMPLEMENT(0x65CF00u, Seek)
		IMPLEMENT(0x65D0D0u, GetFileSize)
		IMPLEMENT(0x65CDD0u, WriteBytes)
		IMPLEMENT(0x65CCA0u, Close)
		IMPLEMENT(0x65D1F0u, GetFileTime)
		IMPLEMENT(0x65D240u, SetFileTime)
		IMPLEMENT(0x65CA70u, CDCheck)
		IMPLEMENT(0x65D2B0u, Bias)
	}
};
DEFINE_IMPL_END(RawFileClass)
