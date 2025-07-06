# Compiler
CXX := g++

# Directories
BUILD_DIR := build
SRC_DIR := src
LIB_DIR := lib
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
FMOD_DIR := fmod/core

# Static library
LIB_NAME := launchpad
STATIC_LIB := $(LIB_DIR)/lib$(LIB_NAME).a
FMOD_LIB := $(FMOD_DIR)/lib/libfmod.dylib
FMOD_LIB_LINK := $(LIB_DIR)/libfmod.dylib

# Source & Headers
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INCLUDE_PATHS := $(FMOD_DIR)/include include $(SRC_DIR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler & Linker Flags
CXXFLAGS := -std=c++20 -Wall -Wextra $(INCLUDE_PATHS:%=-I%)
LDFLAGS := -L$(FMOD_DIR)/lib -lfmod

# Dependency files
DEPS := $(OBJ:.o=.d)

# Default target
static: $(STATIC_LIB) $(FMOD_LIB_LINK)

# Create lib archive
$(STATIC_LIB): $(OBJ) | $(LIB_DIR)
	@echo "[archive] lib$(LIB_NAME).a"
	@ar rcs "$@" $(OBJ)

# Link fmod lib
$(FMOD_LIB_LINK): $(FMOD_LIB) | $(LIB_DIR)
	@echo "[link dependency] libfmod.dylib"
	@[ ! -L "$@" ] || rm "$@"
	@# Link against relative fmod lib path
	@ln -s "../$(FMOD_LIB)" "$@"

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "[compile] $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$<)"
	@"$(CXX)" $(CXXFLAGS) -MMD -MF "$(OBJ_DIR)/$*.d" -c "$<" -o "$@"

# Create necessary directories
$(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)" "$(LIB_DIR)"

# Include dependency files
-include $(DEPS)

.PHONY: static clean
