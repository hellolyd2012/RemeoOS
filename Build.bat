@echo off
@title RemeoOS编译
@mkdir Build
@cd Build
@mkdir EFI
@cd EFI
@mkdir Boot
@cd ..
@cd ..
@g++ -W -Wno-missing-field-initializer -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Wextra -e BootLoaderMain -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -o Build\EFI\Boot\BootX64.efi Boot\Boot.cpp -I Includes
@x86_64-elf-g++.exe -O3 -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -ffreestanding -fno-exceptions -fno-rtti -std=c++23 -c Kernel\Kernel.cpp -o Build\Kernel.o -I Includes
@x86_64-elf-ld.exe -e KernelMain  -z norelro --static -o Build\Kernel.elf Build\Kernel.o -z norelro
@qemu-system-x86_64 -bios OVMF.fd -m 1024 -net none -drive file=fat:rw:Build,index=0,format=vvfat -serial file:Run.log -device usb-ehci,id=usb -device usb-tablet,bus=usb.0 -device usb-kbd,bus=usb.0
@echo on