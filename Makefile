# Compiler
CXX := g++

# Directories
BUILD_DIR := build
SRC_DIR := src
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
FMOD_DIR := fmod/core

# Executable
PROJ := launchpad
EXE := $(BIN_DIR)/$(PROJ)
MAIN_SRC := $(SRC_DIR)/main.cpp

# Source & Headers
SRC := $(patsubst $(MAIN_SRC),,$(wildcard $(SRC_DIR)/*.cpp))
INCLUDE_PATHS := $(FMOD_DIR)/include $(SRC_DIR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler & Linker Flags
CXXFLAGS := -std=c++20 -Wall -Wextra $(INCLUDE_PATHS:%=-I%)
LDFLAGS := -L$(FMOD_DIR)/lib -lfmod -Wl,-rpath,$(FMOD_DIR)/lib

# Dependency files
DEPS := $(OBJ:.o=.d)

# Default target
exe: $(EXE)

# Compile executable
$(EXE): $(MAIN_SRC) $(OBJ) | $(BIN_DIR)
	@"$(CXX)" $(CXXFLAGS) $(OBJ) "$<" -o "$@" $(LDFLAGS)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "[compile] $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$<)"
	@"$(CXX)" $(CXXFLAGS) -MMD -MF "$(OBJ_DIR)/$*.d" -c "$<" -o "$@"

# Create necessary directories
$(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)"

# Include dependency files
-include $(DEPS)

.PHONY: all clean test
