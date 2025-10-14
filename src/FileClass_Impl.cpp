#include "FileClass_Impl.h"

void RawFileClass_Impl::DTOR()
{
	reset_vtable();
	if (This->HasHandle())
	{
		if (!CloseHandle(This->Handle))
		{
			This->CDCheck(GetLastError(), 0, This->FileName);
		}
		This->Handle = INVALID_HANDLE_VALUE;
	}
	if (This->FileNameAllocated && This->FileName)
	{
		CRT::free(This->FileName);
		This->FileName = nullptr;
		This->FileNameAllocated = false;
	}
	BASE_CLASS_DTOR(FileClass);
}


const char* RawFileClass_Impl::SetFileName(const char* pFileName)
{
	const char* result;

	if (This->FileName && This->FileNameAllocated)
	{
		CRT::free(This->FileName);
		This->FileName = nullptr;
		This->FileNameAllocated = false;
	}

	if (!pFileName)
		return 0;

	This->FilePointer = 0;
	This->FileSize = INVALID_FILE_SIZE;
	result = CRT::strdup(pFileName);
	This->FileName = result;
	if (result)
	{
		This->FileNameAllocated = true;
	}
	else
	{
		This->CDCheck(ERROR_INVALID_ACCESS, 0, pFileName);
		result = 0;
	}
	return result;
}