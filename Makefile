# Compiler for ARM and Windows
CC_ARM = arm-none-eabi-gcc
CC_WIN = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests
UNITY_DIR = lib/unity
STARTUP_DIR = startup

# Files
SRCS = $(SRC_DIR)/protection_overload.c
TEST_SRCS = $(TESTS_DIR)/test_protection_overload.c
UNITY_SRC = $(UNITY_DIR)/unity.c
STUBS = $(TESTS_DIR)/stubs.c  # Stubs required for ARM build only
STARTUP_SRCS = $(STARTUP_DIR)/startup.c # Required for ARM build only

# Output Executables
OUT_ARM = $(BUILD_DIR)/test_protection_overload_arm.elf
OUT_WIN = $(BUILD_DIR)/test_protection_overload_win.exe

# Compiler Flags
CFLAGS = -I$(SRC_DIR) -Wall -Wextra -std=c11
LDFLAGS_ARM = -T $(STARTUP_DIR)/linker.ld -nostartfiles -lm --specs=nosys.specs
LDFLAGS_WIN = -lm  # No special specs needed for Windows

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build-only targets
build_arm: $(BUILD_DIR) $(OUT_ARM)

build_win: $(BUILD_DIR) $(OUT_WIN)

# Test targets (build + run)
test_arm: build_arm
	$(OUT_ARM)

test_win: build_win
	$(OUT_WIN)

# Build both versions (ARM & Windows)
build_all: build_arm build_win

# Build & run both
test_all: test_arm test_win

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.exe

# ARM Build
$(OUT_ARM): $(SRCS) $(TEST_SRCS) $(STUBS) $(UNITY_SRC) $(STARTUP_SRCS)
	$(CC_ARM) $(CFLAGS) -I $(UNITY_DIR) -o $@ $^ $(LDFLAGS_ARM)

# Windows Build (excluding stubs)
$(OUT_WIN): $(SRCS) $(TEST_SRCS) $(UNITY_SRC)
	$(CC_WIN) $(CFLAGS) -I $(UNITY_DIR) -o $@ $^ $(LDFLAGS_WIN)
