// 创建于2024.10.20
// RemeoOS 内存

#ifndef _MEMORY_H_
#define _MEMORY_H_

#define FREE_MEMORY 1
#define MMIO_MEMORY 2
#define UEFI_MEMORY 3
#pragma pack(16)
#include "UEFI/UEFI.hpp"

typedef struct __packed
{
    UINTN MapSize;
    VOID *Buffer;
    UINTN MapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
} MEMORY_MAP;

typedef struct {
	UINT32 Type;
	UINT64 PhysicalStart;
	UINT64 PageSize;
} OS_MEMORY_DESCRIPTOR;
#endif