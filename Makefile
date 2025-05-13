# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Icore -Ischedulers/uniprocessor -Ischedulers/multiprocessor -Isim -Ifeasibility -Igenerator
DEBUG_FLAGS := -g -O0 -fno-inline -fno-omit-frame-pointer -ggdb -fno-elide-constructors

# Output folder
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/rts-sat

# Source files
SRCS := $(wildcard core/*.cpp) \
        $(wildcard schedulers/uniprocessor/*.cpp) \
        $(wildcard schedulers/multiprocessor/*.cpp) \
        $(wildcard sim/*.cpp) \
        $(wildcard feasibility/*.cpp) \
        $(wildcard generator/*.cpp) \
        main.cpp

# Object files (build/foo.o for foo.cpp)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# ==== GTEST CONFIG ====
GTEST_DIR := third_party/gtest/googletest
GTEST_SRC := $(GTEST_DIR)/src/gtest-all.cc
GTEST_OBJ := $(BUILD_DIR)/$(GTEST_SRC:.cc=.o)
GTEST_MAIN_SRC := $(GTEST_DIR)/src/gtest_main.cc
GTEST_MAIN_OBJ := $(BUILD_DIR)/$(GTEST_MAIN_SRC:.cc=.o)
GTEST_INCLUDES := -I$(GTEST_DIR) -I$(GTEST_DIR)/include

# ==== TESTS ====
TEST_SRCS := $(wildcard core/*.cpp) \
        $(wildcard schedulers/uniprocessor/*.cpp) \
        $(wildcard schedulers/multiprocessor/*.cpp) \
        $(wildcard sim/*.cpp) \
        $(wildcard feasibility/*.cpp) \
        $(wildcard generator/*.cpp) \
		$(wildcard tests/core/*.cpp) \
		$(wildcard tests/schedulers/uniprocessor/*.cpp) \
        $(wildcard tests/schedulers/multiprocessor/*.cpp) \
        $(wildcard tests/sim/*.cpp) \
        $(wildcard tests/feasibility/*.cpp) \
        $(wildcard tests/generator/*.cpp)

TEST_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

TEST_TARGET := $(BIN_DIR)/tests

# Default target
all: CXXFLAGS += -O2
all: $(TARGET)


# Debug target
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)


# Link the main binary
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^


# ==== Build test binary ====
test: CXXFLAGS += $(GTEST_INCLUDES) -pthread
test: $(TEST_TARGET)
	@if [ -f $(TEST_TARGET) ]; then ./$(TEST_TARGET); fi

$(TEST_TARGET): $(TEST_OBJS) $(GTEST_OBJ) $(GTEST_MAIN_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread


# Compile each cpp file
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GTEST_OBJ): $(GTEST_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) -c $< -o $@

$(GTEST_MAIN_OBJ): $(GTEST_MAIN_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDES) -c $< -o $@


# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


# Phony targets
.PHONY: all debug test clean
