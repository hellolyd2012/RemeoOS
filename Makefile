OS_ARCH := X86

BUILD_DIR := Build
KERNEL_DIR := Kernel
OBJECT_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
ISO_DIR := $(BUILD_DIR)/iso
ISO_BOOT_DIR := $(ISO_DIR)/boot
ISO_GRUB_DIR := $(ISO_BOOT_DIR)/grub

INCLUDES_DIR := Includes
INCLUDES := $(patsubst %, -I %, $(INCLUDES_DIR))

OS_NAME = RemeoOS
OS_BIN = $(OS_NAME).bin
OS_ISO = $(OS_NAME).iso

CC := gcc
AS := g++
PP := g++

O := -O3
W := #-Wall -Wextra #启用警告
CFLAGS := -std=c++17 -ffreestanding $(O) $(W)
LDFLAGS := -ffreestanding $(O) -nostdlib

SOURCE_FILES := $(shell find -name "*.cpp" -o -name "*.S")
SRC := $(patsubst ./%, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(ISO_DIR):
	@mkdir -p $(ISO_DIR)
	@mkdir -p $(ISO_BOOT_DIR)
	@mkdir -p $(ISO_GRUB_DIR)

$(OBJECT_DIR)/%.S.o: %.S
	@mkdir -p $(@D)
	@$(PP) $(INCLUDES) -c $< -o $@ -m32
	@echo "AS" $<

$(OBJECT_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	@$(PP) $(INCLUDES) -c $< -o $@ $(CFLAGS) -m32
	@echo "PP" $<

$(BIN_DIR)/$(OS_BIN): $(OBJECT_DIR) $(BIN_DIR) $(SRC)
	@$(CC) -T Linker.ld -o $(BIN_DIR)/$(OS_BIN) $(SRC) $(LDFLAGS) -m32
	@echo "LD" $<

$(BUILD_DIR)/$(OS_ISO): $(ISO_DIR) $(BIN_DIR)/$(OS_BIN) GRUB_TEMPLATE
	@./config-grub.sh ${OS_NAME} > $(ISO_GRUB_DIR)/grub.cfg
	@cp $(BIN_DIR)/$(OS_BIN) $(ISO_BOOT_DIR)
	@grub-mkrescue -o $(BUILD_DIR)/$(OS_ISO) $(ISO_DIR)

all: clean $(BUILD_DIR)/$(OS_ISO)

all-debug: O := -O0
all-debug: CFLAGS := -g -std=c++17 -ffreestanding $(O) $(W) -fomit-frame-pointer
all-debug: LDFLAGS := -ffreestanding $(O) -nostdlib -m32
all-debug: clean $(BUILD_DIR)/$(OS_ISO)
	@objdump -D $(BIN_DIR)/$(OS_BIN) > dump

clean:
	@rm -rf $(BUILD_DIR)
	@clear

run: $(BUILD_DIR)/$(OS_ISO)
	@qemu-system-i386 -cdrom $(BUILD_DIR)/$(OS_ISO)

debug-qemu: all-debug
	@objcopy --only-keep-debug $(BIN_DIR)/$(OS_BIN) $(BUILD_DIR)/kernel.dbg
	@qemu-system-i386 -s -S -kernel $(BIN_DIR)/$(OS_BIN) &
	@gdb -s $(BUILD_DIR)/kernel.dbg -ex "target remote localhost:1234"

debug-bochs: all-debug
	@bochs -q -f Bochs.cfg

Config:
	@chmod u+x config-grub.sh