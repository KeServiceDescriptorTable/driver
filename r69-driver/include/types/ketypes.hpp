#ifndef ketypes_hpp
#define ketypes_hpp

#include <framework.hpp>

typedef struct _KLDR_DATA_TABLE_ENTRY {
    struct _LIST_ENTRY InLoadOrderLinks;
    void* ExceptionTable;
    uint32_t ExceptionTableSize;
    void* GpValue;
    struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
    void* DllBase;
    void* EntryPoint;
    uint32_t SizeOfImage;
    struct _UNICODE_STRING FullDllName;
    struct _UNICODE_STRING BaseDllName;
    uint32_t Flags;
    uint16_t LoadCount;
    union {
        uint16_t SignatureLevel : 4;
        uint16_t SignatureType : 3;
        uint16_t Unused : 9;
        uint16_t EntireField;
    } u1;
    void* SectionPointer;
    uint32_t CheckSum;
    uint32_t CoverageSectionSize;
    void* CoverageSection;
    void* LoadedImports;
    void* Spare;
    uint32_t SizeOfImageNotRounded;
    uint32_t TimeDateStamp;
}KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

typedef struct _KTHREAD {
    struct _DISPATCHER_HEADER Header;
    uint8_t pad_0x68_0[0x68];
    uint32_t SystemCallNumber;
    uint32_t ReadyTime;
    void* FirstArgument;
    struct _KTRAP_FRAME* TrapFrame;
}KTHREAD, * PKTHREAD;

typedef struct _KPROCESS {
    struct _DISPATCHER_HEADER Header;
    struct _LIST_ENTRY ProfileListHead;
    uint64_t DirectoryTableBase;
}KPROCESS, *PKPROCESS;

#endif // !ketypes_hpp