# Compiler for ARM and Windows
CC_ARM = arm-none-eabi-gcc
CC_WIN = gcc

# Emulation
QEMU = qemu-system-arm
QEMU_MACHINE = lm3s6965evb
# QEMU_FLAGS = -semihosting -nographic
# QEMU_FLAGS = -semihosting-config enable=on,target=native -nographic -d in_asm,guest_errors
# QEMU_FLAGS = -semihosting-config enable=on,target=native -nographic -serial mon:stdio -d in_asm,guest_errors
QEMU_FLAGS = -semihosting-config enable=on,target=auto -nographic -serial mon:stdio -d in_asm,guest_errors -D qemu.log

# GDB
GDB = arm-none-eabi-gdb

# Directories
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests
UNITY_DIR = lib/unity
STARTUP_DIR = startup

# Files
SRCS = $(SRC_DIR)/protection_overload.c
TEST_SRCS = $(TESTS_DIR)/test_protection_overload.c
MOCK_SRCS = $(TESTS_DIR)/mock_sensor.c
UNITY_SRC = $(UNITY_DIR)/unity.c
STUBS = $(TESTS_DIR)/stubs.c  # Stubs required for ARM build only
STARTUP_SRCS = $(STARTUP_DIR)/startup.c # Required for ARM build only

# Output Executables
OUT_ARM = $(BUILD_DIR)/test_protection_overload_arm.elf
OUT_WIN = $(BUILD_DIR)/test_protection_overload_win.exe

# Compiler Flags
CFLAGS = -I$(SRC_DIR) -I$(TESTS_DIR) -Wall -Wextra -std=c11
CFLAGS += -g
CFLAGS_ARM = -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -march=armv7-m -specs=rdimon.specs -lc -lrdimon
LDFLAGS_ARM = -T $(STARTUP_DIR)/linker.ld -nostartfiles -lm --specs=nosys.specs -Wl,-Map=$(BUILD_DIR)/test_protection_overload_arm.map
LDFLAGS_WIN = -lm  # No special specs needed for Windows

# GDB Commands file
GDB_CMD_FILE = $(BUILD_DIR)/gdb_commands.txt

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build-only targets
build_arm: $(BUILD_DIR) $(OUT_ARM)

build_win: $(BUILD_DIR) $(OUT_WIN)

# Test targets (build + run)
test_arm: build_arm
	@echo "Running ARM tests with QEMU..."
	@$(QEMU) -M $(QEMU_MACHINE) -kernel $(OUT_ARM) $(QEMU_FLAGS)

test_win: build_win
	@echo "Running Windows tests..."
	$(OUT_WIN)

# QEMU debug target
debug_arm: build_arm $(GDB_CMD_FILE)
	@echo "Starting QEMU in debug mode on port 1234..."
	@$(QEMU) -M $(QEMU_MACHINE) -kernel $(OUT_ARM) $(QEMU_FLAGS) -S -gdb tcp::1234 &
	@echo "Connecting GDB..."
	@$(GDB) -x $(GDB_CMD_FILE) $(OUT_ARM)
	@echo "Debug session ended."

# Create GDB commands file
$(GDB_CMD_FILE): $(BUILD_DIR)
	@echo "target remote localhost:1234" > $@
	@echo "monitor system_reset" >> $@
	@echo "break main" >> $@
	@echo "break _puts_r " >> $@
	@echo "break __sfvwrite_r" >> $@
	@echo "break _write" >> $@
	@echo "break strlen" >> $@
	@echo "continue" >> $@

# Build both versions (ARM & Windows)
build_all: build_arm build_win

# Build & run both
test_all: test_arm test_win

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.exe $(BUILD_DIR)/*.txt $(BUILD_DIR)/*.map

# ARM Build (now includes mock sensor)
$(OUT_ARM): $(SRCS) $(TEST_SRCS) $(MOCK_SRCS) $(STUBS) $(UNITY_SRC) $(STARTUP_SRCS)
	$(CC_ARM) $(CFLAGS) $(CFLAGS_ARM) -I $(UNITY_DIR) -o $@ $^ $(LDFLAGS_ARM)

# Windows Build (including mock sensor but excluding stubs)
$(OUT_WIN): $(SRCS) $(TEST_SRCS) $(MOCK_SRCS) $(UNITY_SRC)
	$(CC_WIN) $(CFLAGS) -I $(UNITY_DIR) -o $@ $^ $(LDFLAGS_WIN)

#.PHONY: build_arm build_win test_arm test_win debug_arm build_all test_all clean


