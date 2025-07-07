# Compiler
CXX := g++

# Directories
BUILD_DIR := build
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := launchpad
THIRD_PARTY_DIR := $(LIB_DIR)/third-party

# Libraries
LIB_NAME := launchpad
STATIC_LIB := $(LIB_DIR)/lib/lib$(LIB_NAME).a
THIRD_PARTY_LIB_PATHS := $(shell find $(THIRD_PARTY_DIR) -name 'lib')/$(shell uname -m)
THIRD_PARTY_PATHS := $(patsubst %/lib,%,$(shell find $(THIRD_PARTY_DIR) -name 'lib'))
THIRD_PARTY_LIBS := $(patsubst $(THIRD_PARTY_DIR)/%,%,$(THIRD_PARTY_PATHS))

# Executable
PROJ := launchpad-demo
EXE := $(BIN_DIR)/$(PROJ)
MAIN_SRC := $(SRC_DIR)/main.cpp

# Distribution dirs
DIST_DIR := dist/$(PROJ)
DIST_LIB_DIR := $(DIST_DIR)/lib
DIST_LICENSE_DIR := $(DIST_DIR)/licenses

# Source & Headers
SRC := $(patsubst $(MAIN_SRC),,$(wildcard $(SRC_DIR)/*.cpp))
INCLUDE_PATHS := $(LIB_DIR)/include $(SRC_DIR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler & Linker Flags
CXXFLAGS := -std=c++20 -Wall -Wextra $(addprefix -I, $(INCLUDE_PATHS))
LDFLAGS := $(addprefix -L, $(LIB_DIR)/lib $(THIRD_PARTY_LIB_PATHS)) \
					 $(addprefix -l, $(LIB_NAME) $(THIRD_PARTY_LIBS)) \
					 $(THIRD_PARTY_LIB_PATHS:%=-Wl,-rpath,%) \
					 -Wl,-rpath,@loader_path/lib

# Dependency files
DEPS := $(OBJ:.o=.d)

# Default target
exe: $(EXE)

# Create distribution
bundle: $(EXE) | $(DIST_LIB_DIR) $(DIST_LICENSE_DIR)
	@echo "[bundle] $(PROJ)"
	@# Copy executable
	@cp "$(EXE)" "$(DIST_DIR)"
	@# Copy audio files
	@cp -r audio "$(DIST_DIR)"
	@# Copy dynamic libs
	@find "$(THIRD_PARTY_LIB_PATHS)" -name '*[^L].dylib' -exec cp {} "$(DIST_LIB_DIR)" \;
	@# Copy relevant licenses
	@find "$(THIRD_PARTY_PATHS)" -name '*license*.txt' -exec cp {} "$(DIST_LICENSE_DIR)" \;
	@# Create release zip
	@cd "$(DIST_DIR)/.." && zip -r "$(PROJ)-macos.zip" "$(PROJ)" -x '*.DS_Store'

# Compile executable
$(EXE): $(MAIN_SRC) $(OBJ) | $(BIN_DIR) $(STATIC_LIB)
	@echo "[compile] $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$<)"
	@"$(CXX)" $(CXXFLAGS) $(OBJ) "$<" -o "$@" $(LDFLAGS)

# Create static library
$(STATIC_LIB):
	@echo "=== [build] lib$(LIB_NAME).a ==="
	@$(MAKE) -C "$(LIB_DIR)"
	@echo "=== [done] lib$(LIB_NAME).a ==="

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "[compile] $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$<)"
	@"$(CXX)" $(CXXFLAGS) -MMD -MF "$(OBJ_DIR)/$*.d" -c "$<" -o "$@"

# Create necessary directories
$(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR) $(DIST_LIB_DIR) $(DIST_LICENSE_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)" "$(BIN_DIR)" "$(DIST_DIR)"

# Include dependency files
-include $(DEPS)

.PHONY: exe bundle clean
