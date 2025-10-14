#pragma once
#include <windows.h>
#include <initializer_list>
#include <cstring>
#include <memory>

// ʹ�ø����صĽ������ƣ������ͻ
#define CODE_PATCH_SECTION ".codepatch"
#pragma section(CODE_PATCH_SECTION, read, execute)

// ָ��ṹ��
#pragma pack(push, 1)
struct FarJumpInstruction
{
    BYTE opcode;        // 0xE9 for near jump, 0xEA for far jump
    DWORD offset;       // ��תƫ����

    FarJumpInstruction(DWORD from, DWORD to)
    {
        opcode = 0xE9;
        offset = to - from - 5;
    }
};

struct NearCallInstruction
{
    BYTE opcode;        // 0xE8
    DWORD offset;       // ����ƫ����

    NearCallInstruction(DWORD from, DWORD to)
    {
        opcode = 0xE8;
        offset = to - from - 5;
    }
};

struct ExtendedCallInstruction
{
    WORD prefix;        // �������ǰ׺
    DWORD offset;       // ����ƫ����

    ExtendedCallInstruction(DWORD from, DWORD to)
    {
        prefix = 0x15FF; // FF15 ������
        offset = to;
    }
};
#pragma pack(pop)

// �ڴ汣����װ��
class MemoryGuard
{
private:
    void* m_address;
    size_t m_size;
    DWORD m_oldProtect;
    bool m_locked;

public:
    MemoryGuard(void* address, size_t size, DWORD newProtect = PAGE_EXECUTE_READWRITE)
        : m_address(address), m_size(size), m_locked(false)
    {
        if (VirtualProtect(m_address, m_size, newProtect, &m_oldProtect)) {
            m_locked = true;
        }
    }

    ~MemoryGuard()
    {
        if (m_locked) {
            DWORD temp;
            VirtualProtect(m_address, m_size, m_oldProtect, &temp);
        }
    }

    // ���ø���
    MemoryGuard(const MemoryGuard&) = delete;
    MemoryGuard& operator=(const MemoryGuard&) = delete;
};

// ������Ϣ�ṹ
struct SectionInfo
{
    void* virtualAddress;
    size_t virtualSize;
};

// ��������
#pragma pack(push, 1)
class CodeModifier
{
private:
    DWORD m_targetOffset;
    DWORD m_dataSize;
    const BYTE* m_patchData;

public:
    // Ĭ�Ϲ��캯��
    CodeModifier() : m_targetOffset(0), m_dataSize(0), m_patchData(nullptr) {}

    // ���������캯��
    CodeModifier(DWORD offset, DWORD size, const BYTE* data)
        : m_targetOffset(offset), m_dataSize(size), m_patchData(data) {}

    // Ӧ�ò������ڴ�
    void Execute() const
    {
        if (m_targetOffset == 0 || m_dataSize == 0 || m_patchData == nullptr) {
            return;
        }

        void* targetAddress = reinterpret_cast<void*>(m_targetOffset);

        // ʹ��RAIIģʽ�����ڴ汣��
        MemoryGuard guard(targetAddress, m_dataSize);

        // ��ȫ�ظ�������
        std::memcpy(targetAddress, m_patchData, m_dataSize);
    }

    // ��̬������
    static void ExecuteAllStored();

    // ���ͻ����ݲ���
    template <typename DataType>
    static void ModifyWithData(DWORD offset, std::initializer_list<DataType> dataPattern)
    {
        CodeModifier modifier(
            offset,
            static_cast<DWORD>(dataPattern.size() * sizeof(DataType)),
            reinterpret_cast<const BYTE*>(dataPattern.begin())
        );
        modifier.Execute();
    }

    // ԭʼ�ֽڲ���������汾��
    template <size_t ArraySize>
    static void ModifyRawBytes(DWORD offset, const BYTE(&byteArray)[ArraySize])
    {
        CodeModifier modifier(offset, static_cast<DWORD>(ArraySize), byteArray);
        modifier.Execute();
    }

    // ԭʼ�ֽڲ�������ʼ���б�汾��
    static void ModifyRawBytes(DWORD offset, std::initializer_list<BYTE> byteData)
    {
        ModifyWithData<BYTE>(offset, byteData);
    }

    // ԭʼ�ֽڲ�����ָ��+���Ȱ汾��
    static void ModifyRawBytes(DWORD offset, const BYTE* dataPtr, size_t dataLength)
    {
        CodeModifier modifier(offset, static_cast<DWORD>(dataLength), dataPtr);
        modifier.Execute();
    }

    // Զ��ת����
    static void InsertFarJump(DWORD offset, DWORD jumpTarget);
    static void InsertFarJump(DWORD offset, void* jumpTarget)
    {
        InsertFarJump(offset, reinterpret_cast<DWORD>(jumpTarget));
    }

    // �����ò���
    static void InsertNearCall(DWORD offset, DWORD callTarget);
    static void InsertNearCall(DWORD offset, void* callTarget)
    {
        InsertNearCall(offset, reinterpret_cast<DWORD>(callTarget));
    }

    // 6�ֽڵ��ò����������ʽ��
    static void InsertExtendedCall(DWORD offset, DWORD callTarget);
    static void InsertExtendedCall(DWORD offset, void* callTarget)
    {
        InsertExtendedCall(offset, reinterpret_cast<DWORD>(callTarget));
    }

    // �麯�����޲�
    static void UpdateVTableEntry(DWORD vtableOffset, DWORD newFunction);
    static void UpdateVTableEntry(DWORD vtableOffset, void* newFunction)
    {
        UpdateVTableEntry(vtableOffset, reinterpret_cast<DWORD>(newFunction));
    }

    template <typename T>
    static DWORD GetMemberFunctionAddress(T func)
    {
        union {
            T memberFunc;
            DWORD address;
        } converter;

        converter.memberFunc = func;
        return converter.address;
    }

    template <typename T>
    static void UpdateVTableEntry(DWORD vtableOffset, T memberFunc)
    {
        UpdateVTableEntry(vtableOffset, GetMemberFunctionAddress(memberFunc));
    }

    // ƫ�����޲���������
    static void ModifyPointer(DWORD offset, DWORD newValue)
    {
        UpdateVTableEntry(offset, newValue);
    }
    static void ModifyPointer(DWORD offset, void* newValue)
    {
        UpdateVTableEntry(offset, newValue);
    }
};
#pragma pack(pop)

// ����ɨ�躯������
bool LocateExecutableSection(const char* sectionName, SectionInfo* result);