# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Icore -Ischedulers -Isim

# Output folder
BUILD_DIR := build
BIN := $(BUILD_DIR)/rts-sat

# Source files
SRCS := $(wildcard core/*.cpp) \
        $(wildcard schedulers/*.cpp) \
        $(wildcard sim/*.cpp) \
        $(wildcard feasibility/*.cpp) \
        main.cpp

# Object files (build/foo.o for foo.cpp)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN)

# Link the final binary
$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp to a .o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
