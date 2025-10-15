#include "FileClass_Impl.h"
#include <Unsorted.h>

//------------------------------------------
//--------------FileClass_Impl--------------
//------------------------------------------
void* FileClass_Impl::ReadWholeFile()
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