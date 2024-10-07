OS_ARCH := X86
BUILD_DIR := Build
KERNEL_DIR := Kernel

OBJECT_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
ISO_DIR := $(BUILD_DIR)/iso
ISO_BOOT_DIR := $(ISO_DIR)/boot
ISO_GRUB_DIR := $(ISO_BOOT_DIR)/grub

INCLUDES_DIR := Includes
INCLUDES := $(patsubst %, -I%, $(INCLUDES_DIR))

OS_NAME = RemeoOS
OS_BIN = $(OS_NAME).bin
OS_ISO = $(OS_NAME).iso

CC := gcc
AS := gcc
PM := apt

O := -O3
W := -Wall -Wextra

CFLAGS := -std=gnu99 -ffreestanding $(O) $(W) -m32
LDFLAGS := -ffreestanding $(O) -nostdlib 

SOURCE_FILES := $(shell find -name "*.[cS]")
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
	@echo "如果编译中断可以使用make Config配置"
	@mkdir -p $(@D)
	@echo "正在编译"$< "一下是编译信息"
	@$(CC) -c $< -o $@ -m32

$(OBJECT_DIR)/%.c.o: %.c 
	@echo "如果编译中断可以使用make Config配置"
	@mkdir -p $(@D)
	@echo "正在编译"$< "一下是编译信息"
	@$(CC) -I $(INCLUDES_DIR) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/$(OS_BIN): $(OBJECT_DIR) $(BIN_DIR) $(SRC)
	@echo "正在链接"$< "一下是链接信息"
	@$(CC) -T Linker.ld -o $(BIN_DIR)/$(OS_BIN) $(SRC) $(LDFLAGS) -m32

$(BUILD_DIR)/$(OS_ISO): $(ISO_DIR) $(BIN_DIR)/$(OS_BIN) GRUB_TEMPLATE
	@echo "如果生成中断可以使用make Config配置"
	@echo "正在生成Grub配置文件..."
	@./config-grub.sh ${OS_NAME} > $(ISO_GRUB_DIR)/grub.cfg
	@echo "正在生成ISO文件..."
	@cp $(BIN_DIR)/$(OS_BIN) $(ISO_BOOT_DIR)
	@grub-mkrescue -o $(BUILD_DIR)/$(OS_ISO) $(ISO_DIR)

all: Clean $(BUILD_DIR)/$(OS_ISO)
all-debug: O := -O0
all-debug: CFLAGS := -g -std=gnu99 -ffreestanding $(O) $(W) -fomit-frame-pointer -m32
all-debug: LDFLAGS := -ffreestanding $(O) -nostdlib -m32
all-debug: clean $(BUILD_DIR)/$(OS_ISO)
	@echo "正在生成调试文件..."
	@objdump -D $(BIN_DIR)/$(OS_BIN) > dump

Clean:
	@echo "正在清理..."
	@rm -rf $(BUILD_DIR)
	@clear

Run: $(BUILD_DIR)/$(OS_ISO)
	@echo "正在运行..."
	@qemu-system-i386 -cdrom $(BUILD_DIR)/$(OS_ISO)

Debug-qemu: all-debug
	@echo "正在生成调试文件..."
	@objcopy --only-keep-debug $(BIN_DIR)/$(OS_BIN) $(BUILD_DIR)/kernel.dbg
	@qemu-system-i386 -s -S -kernel $(BIN_DIR)/$(OS_BIN) &
	@gdb -s $(BUILD_DIR)/kernel.dbg -ex "target remote localhost:1234"

Debug-Bochs: all-debug
	@echo "正在生成调试文件..."
	@bochs -q -f bochs.cfg

Config: 
	@echo "正在配置..."
	@sudo chmod u+x config-grub.sh
	@sudo $(PM) install bin86 nasm
	@sudo $(PM) install gcc g++
	@sudo $(PM) install make cmake
	@sudo $(PM) install grub-pc xorriso
	@echo "配置完成"
	@echo "请使用make all进行编译"