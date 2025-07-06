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
THIRD_PARTY_LIB_PATHS := $(shell find $(THIRD_PARTY_DIR) -name 'lib')
THIRD_PARTY_PATHS := $(patsubst %/lib,%,$(THIRD_PARTY_LIB_PATHS))
THIRD_PARTY_LIBS := $(patsubst $(THIRD_PARTY_DIR)/%,%,$(THIRD_PARTY_PATHS))

# Executable
PROJ := launchpad-demo
EXE := $(BIN_DIR)/$(PROJ)
MAIN_SRC := $(SRC_DIR)/main.cpp

# Source & Headers
SRC := $(patsubst $(MAIN_SRC),,$(wildcard $(SRC_DIR)/*.cpp))
INCLUDE_PATHS := $(LIB_DIR)/include $(SRC_DIR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler & Linker Flags
CXXFLAGS := -std=c++20 -Wall -Wextra $(addprefix -I, $(INCLUDE_PATHS))
LDFLAGS := $(addprefix -L, $(LIB_DIR)/lib $(THIRD_PARTY_LIB_PATHS)) \
					 $(addprefix -l, $(LIB_NAME) $(THIRD_PARTY_LIBS)) \
					 $(THIRD_PARTY_LIB_PATHS:%=-Wl,-rpath,%)

# Dependency files
DEPS := $(OBJ:.o=.d)

# Default target
exe: $(EXE)

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
$(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)"

# Include dependency files
-include $(DEPS)

.PHONY: exe clean
