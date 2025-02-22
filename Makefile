# Compiler
CC = arm-none-eabi-gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests

# Files
SRCS = $(SRC_DIR)/protection_overload.c
TEST_SRCS = $(TESTS_DIR)/test_protection_overload.c $(TESTS_DIR)/stubs.c
OBJS = $(SRCS:.c=.o)
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Compiler Flags
CFLAGS = -I$(SRC_DIR) -Wall -Wextra -std=c11
LDFLAGS = -lm --specs=nosys.specs

# Build target for testing
$(BUILD_DIR)/test_protection_overload.elf: $(SRCS) $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf

# Run tests
test: $(BUILD_DIR)/test_protection_overload.elf
	$(BUILD_DIR)/test_protection_overload.elf
