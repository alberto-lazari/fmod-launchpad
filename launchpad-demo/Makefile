# Compiler
CXX := g++

# Directories
BUILD_DIR := build
SRC_DIR := src
LIB_DIR := launchpad
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj

# Libraries
LIB_NAME := launchpad
STATIC_LIB := $(LIB_DIR)/lib$(LIB_NAME).a

# Executable
PROJ := launchpad-demo
EXE := $(BIN_DIR)/$(PROJ)
MAIN_SRC := $(SRC_DIR)/main.cpp

# Source & Headers
SRC := $(patsubst $(MAIN_SRC),,$(wildcard $(SRC_DIR)/*.cpp))
INCLUDE_PATHS := $(LIB_DIR)/include $(SRC_DIR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler & Linker Flags
CXXFLAGS := -std=c++20 -Wall -Wextra $(INCLUDE_PATHS:%=-I%)
LDFLAGS := -L$(LIB_DIR)/lib -l$(LIB_NAME) -lfmod -Wl,-rpath,$(LIB_DIR)/lib

# Dependency files
DEPS := $(OBJ:.o=.d)

# Default target
exe: $(EXE)

# Compile executable
$(EXE): $(MAIN_SRC) $(OBJ) | $(BIN_DIR) $(STATIC_LIB)
	@"$(CXX)" $(CXXFLAGS) $(OBJ) "$<" -o "$@" $(LDFLAGS)

# Create static library
$(STATIC_LIB):
	@echo "[build] lib$(LIB_NAME).a"
	@$(MAKE) -C "$(LIB_DIR)"

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

.PHONY: exe clean
