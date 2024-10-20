// 创建于2024.10.20
// RemeoOS BootLoader

// 头文件
#include <UEFI/FrameBuffer.hpp>
#include <UEFI/UEFI.hpp>
#include <Boot/Boot.hpp>
#include <Memory/Memory.hpp>
#include <Exec/Elf.hpp>
#pragma once // 确保只包含一次

extern "C" EFI_STATUS BootLoaderMain(EFI_HANDLE ImageHandle, struct EFI_SYSTEM_TABLE *SystemTable) 
{
    // 初始化UEFI
    efi_init(ImageHandle, SystemTable);

    // 清除屏幕
    ST->ConOut->ClearScreen(ST->ConOut);
    SwitchToResolution(SCR_X, SCR_Y);

    puts((unsigned short *)L"RemeoOS BootLoader V0.1");
    puts((unsigned short *)L"\n");
    puts((unsigned short *)L"Loading Kernel...");
    puts((unsigned short *)L"\n");
    
    puts((unsigned short *)"[SUCCESS] Initializing UEFI\r\n");
    puts((unsigned short *)L"\n");

    EFI_STATUS                status;
    EFI_PHYSICAL_ADDRESS      entry_addr;
    struct EFI_FILE_PROTOCOL *root, *kernel_file;
    UINTN                     kernel_size   = 4194304;
    void                     *kernel_buffer = malloc(kernel_size);

    // 打开文件系统
    status = SFSP->OpenVolume(SFSP, &root);
    // 断言是否成功打开文件系统
    if (EFI_ERROR(status)) 
    {
        puts((unsigned short *)L"[FAIL] Loading File System\r\n");
        puts((unsigned short *)L"\n");
        while (1);
    }
    puts((unsigned short *)L"[SUCCESS] Loading File System\r\n");
    puts((unsigned short *)L"\n");
    
    // 设置内核
    status = root->Open(root, &kernel_file, (CHAR16 *)L"\\Kernel.elf", EFI_FILE_MODE_READ, 0);
    // 断言是否设置内核
    if (EFI_ERROR(status)) 
    {
        puts((unsigned short *)L"[FAIL] Loading OS Files\r\n");
        while (EFI_ERROR(status))
        {
            status = root->Open(root, &kernel_file, (CHAR16 *)"\\Kernel.elf", EFI_FILE_MODE_READ, 0);
        }
    }
    // 读取内核
    status = kernel_file->Read(kernel_file, &kernel_size,kernel_buffer); // 读取内核，直接读到kernel_buffer
    // 断言是否成功读取内核
    if (EFI_ERROR(status)) 
    {
        puts((unsigned short *)L"[FAIL] Loading OS Files\r\n");
        puts((unsigned short *)L"\n");
        while (1);
    }
    puts((unsigned short *)L"[SUCCESS] Loading OS Files\r\n");
    puts((unsigned short *)L"\n");

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)kernel_buffer;
    UINT64      kernel_first_addr, kernel_last_addr;                   // 计算的首尾
    CalcLoadAddressRange(ehdr, &kernel_first_addr, &kernel_last_addr); // 计算范围

    // 加载显卡驱动
    CopyLoadSegments(ehdr);
    entry_addr = ehdr->e_entry;
    
    struct FrameBufferConfig config = 
    {
        (UINT8 *)GOP->Mode->FrameBufferBase, GOP->Mode->Info->PixelsPerScanLine,
        GOP->Mode->Info->HorizontalResolution, GOP->Mode->Info->VerticalResolution    
    };

    // 获取屏幕信息
    switch (GOP->Mode->Info->PixelFormat) 
    {
        case PixelRedGreenBlueReserved8BitPerColor: 
            config.pixel_format = kRGBR; break;
        case PixelBlueGreenRedReserved8BitPerColor: 
            config.pixel_format = kBGRR; break;
        default:
            // 断言是否成功获取屏幕信息
            puts((unsigned short *)L"[ FAIL ] Loading Graphics\r\n");
            puts((unsigned short *)L"\n");
            while (1);
    }
    puts((unsigned short *)L"[ SUCCESS ] Loading Graphics\r\n");
    puts((unsigned short *)L"\n");
    
    BOOT_CONFIG BootConfig;
    BootConfig.MemoryMap.MapSize = 4096;
    BootConfig.MemoryMap.Buffer = NULL;
    BootConfig.MemoryMap.MapKey = 0;
    BootConfig.MemoryMap.DescriptorSize = 0;
    BootConfig.MemoryMap.DescriptorVersion = 0;
    
    // 获取内存信息
    GetMMP(&BootConfig.MemoryMap);
    puts((unsigned short *)L"[ SUCCESS ] Getting Memory Map\r\n");
    puts((unsigned short *)L"\n");
    
    // 跳转至内核
    Kernel kernel = (Kernel)entry_addr;
    puts((unsigned short *)L"Operating System Boot Success.\r\n");
    puts((unsigned short *)L"\n");
    kernel(&config, SystemTable, &BootConfig); // 跳转至内核

    while (1)
    {
        
    }  
}