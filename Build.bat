@echo off
@title RemeoOS编译
@mkdir Build
@g++ -W -Wno-missing-field-initializer -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -Wextra -e BootLoaderMain -nostdinc -nostdlib -fno-builtin -Wl,--subsystem,10 -o Build\BootX64.efi Boot\Boot.cpp -I Includes
@g++ -O3 -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable -ffreestanding -fno-exceptions -fno-rtti -std=c++23 -c Kernel\Kernel.cpp -o Build\Kernel.o -I Includes
ld -e KernelMain --static -o Build\kernel.elf Build\Kernel.o
qemus-sytem-x86_64 -bios OVMF.fd -m 1024 -net none -drive file=fat:rw:Build,index=0,format=vvfat -serial file:serial.log -device usb-ehci,id=usb -device usb-tablet,bus=usb.0 -device usb-kbd,bus=usb.0