#include "FileClass_Impl.h"
#include <Unsorted.h>

//------------------------------------------
//------------RawFileClass_Impl-------------
//------------------------------------------
RawFileClass* RawFileClass_Impl::CTOR(const char* pFileName)
{
	//Initialize values
	FileSize = INVALID_FILE_SIZE;
	Handle = INVALID_HANDLE_VALUE;
	SkipCDCheck = false;
	FileAccess = FileAccessMode::None;
	FilePointer = 0;
	unknown_short_1C = 0;
	unknown_short_1E = 0;

	//NOTE : there is no copy
	//so please ensure pFileName has a long enough lifetime
	FileNameAllocated = false;
	FileName = pFileName;

	//CTOR always set vtable to the current class
	reset_vtable();
	return This;
}

void RawFileClass_Impl::DTOR()
{
	//DTOR always set vtable to the current class
	reset_vtable();

	//first close the file if it's open
	if (This->HasHandle())
	{
		if (!CloseHandle(Handle))
		{
			This->CDCheck(GetLastError(), 0, FileName);
		}
		Handle = INVALID_HANDLE_VALUE;
	}

	//then free the filename if it was allocated
	if (FileNameAllocated && FileName)
	{
		CRT::free(FileName);
		FileName = nullptr;
		FileNameAllocated = false;
	}

	//then Destroy the base class
	BASE_CLASS_DTOR(FileClass);
}

const char* RawFileClass_Impl::GetFileName() const
{
	return FileName;
}

const char* RawFileClass_Impl::SetFileName(const char* pFileName)
{
	//NOTE: unlike the CTOR,
	//here the pFileName argument will be duplicated
	//and let RawFileClass hold the ownership of the copy

	const char* result;

	//Free the old filename if it was allocated
	if (FileName && FileNameAllocated)
	{
		CRT::free(FileName);
		FileName = nullptr;
		FileNameAllocated = false;
	}

	//Check for null filename
	if (!pFileName)
		return nullptr;

	//Reset file state
	FilePointer = 0;
	FileSize = INVALID_FILE_SIZE;

	//Duplicate the new filename
	result = CRT::strdup(pFileName);
	FileName = result;
	if (result)
	{
		FileNameAllocated = true;
	}
	else
	{
		//Memory allocation failed
		This->CDCheck(ERROR_INVALID_ACCESS, 0, pFileName);
		result = nullptr;
	}
	return result;
}

BOOL RawFileClass_Impl::CreateFile()
{
	//If we already have a handle, close it first
	This->Close();

	//Relies on Open(FileAccessMode::Write) to create the file
	if (!This->Open(FileAccessMode::Write))
		return FALSE;

	//Reset file pointer and size
	if (FileSize != INVALID_FILE_SIZE)
		This->Seek(0, FileSeekMode::Set);

	//Closes the handle but keeps the new file
	This->Close();
	return TRUE;
}

BOOL RawFileClass_Impl::DeleteFile()
{
	//If we already have a handle, close it first
	This->Close();

	//If no filename is set, we can't delete it
	if (!FileName)
	{
		This->CDCheck(ERROR_FILE_NOT_FOUND, 0, nullptr);
		return FALSE;
	}

	if (!This->Exists(false))
		return FALSE;

	if (::DeleteFileA(FileName))
		return TRUE;
	else
	{
		This->CDCheck(GetLastError(), 0, FileName);
		return FALSE;
	}
}

bool RawFileClass_Impl::Exists(bool writeShared)
{
	//If no filename is set, it doesn't exist
	if (!FileName)
		return false;
	//If we already have a handle, it exists
	if (This->HasHandle())
		return true;

	HANDLE handle;
	if (!writeShared)
	{
		//For not writeShared files
		//Just try to open it for read access
		handle = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		Handle = handle;
		if (handle == INVALID_HANDLE_VALUE)
			return false;
		if (!CloseHandle(handle))
			This->CDCheck(GetLastError(), 0, FileName);
		Handle = INVALID_HANDLE_VALUE;
		return true;
	}
	else
	{
		//For writeShared files
		// Try to open it for read access with read/write sharing
		This->Open(FileAccessMode::Read);
		if (This->HasHandle())
		{
			if (!CloseHandle(This->Handle))
				This->CDCheck(GetLastError(), 0, FileName);
			Handle = INVALID_HANDLE_VALUE;
			return true;
		}
		return true;
	}
}

bool RawFileClass_Impl::HasHandle() const
{
	return Handle != INVALID_HANDLE_VALUE;
}

bool RawFileClass_Impl::Open(FileAccessMode access)
{
	//If we already have a handle, close it first
	This->Close();

	//If no filename is set, we can't open it
	auto filename = FileName;
	if (!filename)
	{
		This->CDCheck(ERROR_FILE_NOT_FOUND, 0, nullptr);
		return false;
	}
	FileAccess = access;

	//Open the file according to the access mode
	switch (access)
	{
	case FileAccessMode::Read:
		Handle = CreateFileA(filename,
			GENERIC_READ,
			//NOTE : Open(FileAccessMode::Read) enables write sharing
			//related with the writeShared argument
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0,
			OPEN_EXISTING,
			//NOTE : The FILE_FLAG_SEQUENTIAL_SCAN flag is set here
			//to optimize sequential reading
			FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
			0);
		break;
	case FileAccessMode::Write:
		Handle = CreateFileA(filename,
			GENERIC_WRITE,
			0,
			0,
			//NOTE: Create a new file here
			//RawFileClass::CreateFile relies on this behavior
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0);
		break;
	case FileAccessMode::ReadWrite:
		Handle = CreateFileA(filename,
			GENERIC_WRITE | GENERIC_READ,
			0,
			0,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0);
		break;
	default:
		break;
	}
	//reset file pointer and size
	if (FilePointer || FileSize != INVALID_FILE_SIZE)
		This->Seek(0, FileSeekMode::Set);
	//NOTE : HasHandle() Inline Calls <=> Non-virtual Impl Calls
	return HasHandle();
}

bool RawFileClass_Impl::OpenEx(const char* pFileName, FileAccessMode access)
{
	This->SetFileName(pFileName);
	return This->Open(access);
}

int RawFileClass_Impl::ReadBytes(void* pBuffer, int nNumBytes)
{
	DWORD TotalBytesRead = 0;
	DWORD NumberOfBytesRead = 0;
	int nActualNumBytes;
	BYTE* Buffer = reinterpret_cast<BYTE*>(pBuffer);

	//Open the file if it's not already open
	bool OpenedHere = false;
	if (!This->HasHandle())
	{
		if (!This->Open(FileAccessMode::Read))
			return 0;
		OpenedHere = true;
	}

	//Adjust the number of bytes to read if needed
	if (FileSize == INVALID_FILE_SIZE)
	{
		nActualNumBytes = nNumBytes;
	}
	else
	{
		int Position = This->Seek(0, FileSeekMode::Current);
		nActualNumBytes = nNumBytes;
		if (nNumBytes >= FileSize - Position)
			nActualNumBytes = FileSize - Position;
	}

	//Read the data in a loop
	while (nActualNumBytes > 0)
	{
		NumberOfBytesRead = 0;
		if (::ReadFile(Handle, Buffer, nActualNumBytes, &NumberOfBytesRead, 0))
		{
			Buffer += NumberOfBytesRead;
			nActualNumBytes -= NumberOfBytesRead;
			TotalBytesRead += NumberOfBytesRead;
			if (!NumberOfBytesRead)
				break;
		}
		else
		{
			Buffer += NumberOfBytesRead;
			nActualNumBytes -= NumberOfBytesRead;
			TotalBytesRead += NumberOfBytesRead;
			//Set to true in :
			//AudioStream::PlayWAV at 0x407B60 (set to true at 0x407DE1)
			//StreamerThreadFunction at 0x407680 (set to true at 0x4076E2)
			//also set to false afterwards in the functions above.
			if (Game::IsReadingAudioFile)
				break;
			This->CDCheck(GetLastError(), true, FileName);
		}
	}

	//Mark that we are not reading an audio file anymore
	Game::IsReadingAudioFile = false;

	//Close the file if we opened it here
	if (OpenedHere)
		This->Close();

	return TotalBytesRead;
}

int RawFileClass_Impl::DirectSeek(int offset, FileSeekMode seek)
{
	int Position;
	if (This->HasHandle())
	{
		Position = SetFilePointer(Handle, offset, 0, static_cast<DWORD>(seek));
		if (Position == INVALID_SET_FILE_POINTER)
		{
			This->CDCheck(GetLastError(), 0, FileName);
			return 0;
		}
		return Position;
	}
	else
	{
		This->CDCheck(ERROR_INVALID_BLOCK, 0, FileName);
		return 0;
	}
}

int RawFileClass_Impl::FixSeekPointer(int FixedOffset, FileSeekMode FixedSeek, int DirectSeekResult)
{
	int Position;
	DWORD SetFPResult2;
	DWORD ActualFilePointer;

	Position = DirectSeekResult - FilePointer;

	if (Position < 0)
	{
		if (This->HasHandle())
		{
			SetFPResult2 = SetFilePointer(Handle, FilePointer, 0, FILE_BEGIN);
			if (SetFPResult2 == INVALID_SET_FILE_POINTER)
			{
				This->CDCheck(GetLastError(), 0, FileName);
				SetFPResult2 = 0;
			}
		}
		else
		{
			This->CDCheck(ERROR_INVALID_BLOCK, 0, FileName);
			SetFPResult2 = 0;
		}
		Position = SetFPResult2 - FilePointer;
	}

	if (Position > FileSize)
	{
		if (This->HasHandle())
		{
			ActualFilePointer = SetFilePointer(this->Handle, FileSize + FilePointer, 0, FILE_BEGIN);
			if (ActualFilePointer == INVALID_SET_FILE_POINTER)
			{
				This->CDCheck(GetLastError(), 0, This->FileName);
				Position = -FilePointer;
			}
			else Position = ActualFilePointer - FilePointer;
		}
		else
		{
			This->CDCheck(ERROR_INVALID_BLOCK, 0, This->FileName);
			Position = -FilePointer;
		}
	}

	return Position;
}

int RawFileClass_Impl::IndirectSeek(int Offset, FileSeekMode seek)
{
	FileSeekMode FixedSeek;
	int FixedOffset;

	FixedSeek = seek;
	if (seek == FileSeekMode::Set)
		FixedOffset = this->FilePointer + std::min(Offset, FileSize);
	else if (seek == FileSeekMode::End)
	{
		FixedSeek = FileSeekMode::Set;
		FixedOffset = FileSize + this->FilePointer + Offset;
	}
	else
		FixedOffset = Offset;

	return FixSeekPointer(
		FixedOffset,
		FixedSeek,
		DirectSeek(FixedOffset, FixedSeek)
	);
}

int RawFileClass_Impl::Seek(int offset, FileSeekMode seek)
{
	//if we know the file size, use IndirectSeek
	//IndirectSeek ensures the file pointer is always between [FilePointer, FilePointer + FileSize]
	//else use DirectSeek
	if (FileSize != INVALID_FILE_SIZE)
		return IndirectSeek(offset, seek);
	else
		return DirectSeek(offset, seek);
}

int RawFileClass_Impl::GetFileSize()
{
	int Size = 0;
	int result = FileSize;

	if (FileSize != INVALID_FILE_SIZE)
		return result;

	if (This->HasHandle())
	{
		Size = ::GetFileSize(Handle, nullptr);
		if (Size == INVALID_FILE_SIZE)
			This->CDCheck(GetLastError(), 0, FileName);
	}
	else if (This->Open(FileAccessMode::Read))
	{
		Size = This->GetFileSize();
		This->Close();
	}

	result = Size - this->FilePointer;
	this->FileSize = result;
	return result;
}

int RawFileClass_Impl::WriteBytes(void* pBuffer, int nNumBytes)
{
	DWORD result;
	signed int SetFPResult;
	bool OpenedHere = false;
	DWORD NumberOfBytesWritten = 0;

	if (!This->HasHandle())
	{
		result = This->Open(FileAccessMode::Write);
		if (!result)return 0;
		OpenedHere = true;
	}

	if (!::WriteFile(Handle, pBuffer, nNumBytes, &NumberOfBytesWritten, 0))
	{
		This->CDCheck(GetLastError(), 0, FileName);
	}

	if (this->FileSize != INVALID_FILE_SIZE)
	{
		SetFPResult = DirectSeek(0, FileSeekMode::Current);
		if (SetFPResult > FileSize + FilePointer)
		{
			FileSize = DirectSeek(0, FileSeekMode::Current) - FilePointer;
		}
	}

	if (OpenedHere)
		This->Close();

	return NumberOfBytesWritten;
}

void RawFileClass_Impl::Close()
{
	if (This->HasHandle())
	{
		if (!::CloseHandle(This->Handle))
			This->CDCheck(GetLastError(), 0, FileName);
		Handle = INVALID_HANDLE_VALUE;
	}
}

DWORD RawFileClass_Impl::GetFileTime()
{
	WORD FatDate[2], FatTime[2];
	BY_HANDLE_FILE_INFORMATION FileInformation;

	if (!::GetFileInformationByHandle(Handle, &FileInformation))
		return 0;
	::FileTimeToDosDateTime(&FileInformation.ftLastWriteTime, FatDate, FatTime);
	return FatTime[0] | (FatDate[0] << 16);
}

bool RawFileClass_Impl::SetFileTime(DWORD wFatTime)
{
	FILETIME FileTime;
	BY_HANDLE_FILE_INFORMATION FileInformation;

	//NOTE : HasHandle() Inline Calls <=> Non-virtual Impl Calls
	if (HasHandle()
		&& ::GetFileInformationByHandle(Handle, &FileInformation)
		&& ::DosDateTimeToFileTime(HIWORD(wFatTime), (WORD)wFatTime, &FileTime))
	{
		return ::SetFileTime(Handle, &FileInformation.ftCreationTime, &FileTime, &FileTime);
	}
	else return false;
}

void RawFileClass_Impl::Bias(int offset, int length)
{
	if (offset)
	{
		//NOTE : RawFileClass::GetFileSize() Direct Calls <=> Non-virtual Impl Calls
		int size = GetFileSize();
		FileSize = size;
		FilePointer += offset;
		if (length != INVALID_FILE_SIZE)
		{
			if (size >= length)
				size = length;
			this->FileSize = size;
		}

		//Original WW Implementation : FileSize &= (FileSize <= 0) - 1;
		if (FileSize <= 0) FileSize = 0;

		if (This->HasHandle())
			//NOTE : RawFileClass::Seek() Direct Calls <=> Non-virtual Impl Calls
			Seek(0, FileSeekMode::Set);
	}
	else
	{
		this->FilePointer = 0;
		this->FileSize = -1;
	}
}