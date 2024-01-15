# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS += ${DEVENV_FLAGS}

# CFLAGS +=
# CXXFLAGS +=

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
# LDFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)


# Test-related configurations
TEST_SOURCES := $(wildcard src/tps.cpp)  # Adjust with your test file location
# TEST_SOURCES :+ $(wildcard src/tps.cpp)  # Adjust with your test file location
TEST_FLAGS := -DTEST -Isrc  # Define TEST, and include the src directory if needed
TEST_BINARY := testBin  # Name of the test binary


# Test suite target
test: FLAGS += $(TEST_FLAGS) 
test: CXXFLAGS += -Wl,-rpath,/Users/timpierson/arity/am-vcv/sdk/Rack-SDK -L/Users/timpierson/arity/am-vcv/sdk/Rack-SDK -lRack -undefined dynamic_lookup -stdlib=libc++ -mmacosx-version-min=10.9 ${DEVENV_FLAGS}
# test: LDFLAGS += 
test: $(TEST_SOURCES)
	$(CXX) $(FLAGS) $(CXXFLAGS) -o $(TEST_BINARY) $(TEST_SOURCES)
	otool -L $(TEST_BINARY)
	ls -l $(TEST_BINARY) # List the file to confirm it's created
	export DYLD_LIBRARY_PATH=/Users/timpierson/arity/am-vcv/sdk/Rack-SDK:$(DYLD_LIBRARY_PATH)
	./$(TEST_BINARY)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

