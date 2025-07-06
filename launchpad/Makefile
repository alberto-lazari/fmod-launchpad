# Compiler
CXX := g++

# Directories
BUILD_DIR := build
SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := $(BUILD_DIR)/obj
THIRD_PARTY_DIR := third-party

# Static library
LIB_NAME := launchpad
STATIC_LIB := $(LIB_DIR)/lib$(LIB_NAME).a
THIRD_PARTY_LIB_PATHS := $(shell find $(THIRD_PARTY_DIR) -name 'lib')
THIRD_PARTY_PATHS := $(patsubst %/lib,%,$(THIRD_PARTY_LIB_PATHS))
THIRD_PARTY_LIBS := $(patsubst $(THIRD_PARTY_DIR)/%,%,$(THIRD_PARTY_PATHS))

# Source & Headers
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INCLUDE_PATHS := $(addsuffix /include, $(THIRD_PARTY_PATHS)) include $(SRC_DIR)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Compiler & Linker Flags
CXXFLAGS := -std=c++20 -Wall -Wextra $(addprefix -I, $(INCLUDE_PATHS))
LDFLAGS := $(addprefix -L, $(THIRD_PARTY_LIBS)) $(addprefix -l, $(THIRD_PARTY_LIBS))

# Dependency files
DEPS := $(OBJ:.o=.d)

# Default target
static: $(STATIC_LIB)

# Create lib archive
$(STATIC_LIB): $(OBJ) | $(LIB_DIR)
	@echo "[archive] lib$(LIB_NAME).a"
	@ar rcs "$@" $(OBJ)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "[compile] $(patsubst $(SRC_DIR)/%.cpp,%.cpp,$<)"
	@"$(CXX)" $(CXXFLAGS) -MMD -MF "$(OBJ_DIR)/$*.d" -c "$<" -o "$@"

# Create necessary directories
$(BUILD_DIR) $(OBJ_DIR) $(LIB_DIR):
	@mkdir -p "$@"

# Clean rule
clean:
	@rm -rf "$(BUILD_DIR)" "$(LIB_DIR)"

# Include dependency files
-include $(DEPS)

.PHONY: static clean
