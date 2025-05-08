# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Icore -Ischedulers -Isim -Ifeasibility -Igenerator
DEBUG_FLAGS := -g -O0 -fno-inline -fno-omit-frame-pointer -ggdb -fno-elide-constructors

# Output folder
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/rts-sat

# Source files
SRCS := $(wildcard core/*.cpp) \
        $(wildcard schedulers/*.cpp) \
        $(wildcard sim/*.cpp) \
        $(wildcard feasibility/*.cpp) \
        $(wildcard generator/*.cpp) \
        main.cpp

# Object files (build/foo.o for foo.cpp)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: CXXFLAGS += -O2
all: $(TARGET)

# Debug target
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: CCFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Link the final binary
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp to a .o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
