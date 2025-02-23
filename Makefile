# Compiler
CC_ARM = arm-none-eabi-gcc
CC_WIN = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests

# Files
SRCS = $(SRC_DIR)/protection_overload.c
TEST_SRCS = $(TESTS_DIR)/test_protection_overload.c $(TESTS_DIR)/stubs.c

# Compiler Flags
CFLAGS = -I$(SRC_DIR) -Wall -Wextra -std=c11
LDFLAGS_ARM = -lm --specs=nosys.specs
LDFLAGS_WIN = -lm

# Executables
ARM_EXE = $(BUILD_DIR)/test_protection_overload.elf
WIN_EXE = $(BUILD_DIR)/test_protection_overload.exe

# Build ARM
$(ARM_EXE): $(SRCS) $(TEST_SRCS)
	$(CC_ARM) $(CFLAGS) -o $@ $^ $(LDFLAGS_ARM)

# Build Windows
$(WIN_EXE): $(SRCS) $(TEST_SRCS)
	$(CC_WIN) $(CFLAGS) -o $@ $^ $(LDFLAGS_WIN)

# Default: Build both
all: arm win

arm: $(ARM_EXE)
win: $(WIN_EXE)

# Run Windows test
test: win
	$(WIN_EXE)

# Clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.exe


# Use this instruction to test on ARM with QEMU
# qemu-arm -L /path/to/qemu/libs build/test_protection_overload.elf
