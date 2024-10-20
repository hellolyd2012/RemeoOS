// 创建于2024.10.20
// RemeoOS 启动模块

#ifndef _BOOT_H_
#define _BOOT_H_

#include "UEFI/UEFI.hpp"
#include "Memory/Memory.hpp"

typedef struct
{
    MEMORY_MAP MemoryMap;
} BOOT_CONFIG;
#endif