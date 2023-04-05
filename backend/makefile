# author: Brando
# date: 7/29/22
#

include ../common/external/libs/makefiles/libpaths.mk

### Global
BUILD_PATH = build
FILES = plant database user pool pipe driver logger \
instructor instructorversion instructorget instructorsession \
instructorset entity nursery
CXXLINKS = -lmysqlcppconn -lpthread -luuid

### Release settings
R_CXXFLAGS += -I. -I../common/bin/bflib/release/ -I/usr/local/include/cppconn -I../common
R_BIN_NAME = plantdiaryd
R_BUILD_PATH = $(BUILD_PATH)/release
R_MAIN_FILE = src/main.cpp
R_LIBRARIES = ../common/bin/bflib/release/bflibcpp/libbfcpp.a ../common/bin/json.a
R_OBJECTS = $(patsubst %, $(R_BUILD_PATH)/%.o, $(FILES))

### Debug settings
D_CXXFLAGS = -DDEBUG -g -I. -I../common/bin/bflib/debug/ -I/usr/local/include/cppconn -I../common
D_BIN_NAME = $(R_BIN_NAME)-debug
D_BUILD_PATH = $(BUILD_PATH)/debug
D_MAIN_FILE = $(R_MAIN_FILE)
D_LIBRARIES = ../common/bin/bflib/debug/bflibcpp/libbfcpp-debug.a ../common/bin/json.a
D_OBJECTS = $(patsubst %, $(D_BUILD_PATH)/%.o, $(FILES))

### Test settings
T_CXXFLAGS = $(D_CXXFLAGS) -DTESTING -I../common/external/libs/cpplib/testbench/
T_BIN_NAME = $(R_BIN_NAME)-test
T_BUILD_PATH = $(BUILD_PATH)/test
T_MAIN_FILE = src/testbench/tests.cpp
T_LIBRARIES = $(D_LIBRARIES)
T_OBJECTS = $(patsubst %, $(T_BUILD_PATH)/%.o, $(FILES))

### Instructions

# Default
all: release

clean:
	rm -rfv build
	rm -rfv bin

## Release build instructions
release: release-setup bin/$(R_BIN_NAME)

release-setup:
	@mkdir -p $(R_BUILD_PATH)
	@mkdir -p bin

bin/$(R_BIN_NAME): $(R_MAIN_FILE) $(R_OBJECTS) $(R_LIBRARIES)
	g++ -o $@ $^ $(R_CXXFLAGS) $(CXXLINKS) 

$(R_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	g++ -c $< -o $@ $(R_CXXFLAGS)

## Debug build instructions
debug: debug-setup bin/$(D_BIN_NAME)

debug-setup:
	@mkdir -p $(D_BUILD_PATH)
	@mkdir -p bin

bin/$(D_BIN_NAME): $(D_MAIN_FILE) $(D_OBJECTS) $(D_LIBRARIES)
	g++ -o $@ $^ $(D_CXXFLAGS) $(CXXLINKS) 

$(D_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	g++ -c $< -o $@ $(D_CXXFLAGS)

## Test build instructions
test: test-setup bin/$(T_BIN_NAME)
	./bin/$(T_BIN_NAME)

test-setup:
	@mkdir -p $(T_BUILD_PATH)
	@mkdir -p bin

bin/$(T_BIN_NAME): $(T_MAIN_FILE) $(T_OBJECTS) $(T_LIBRARIES)
	g++ -o $@ $^ $(T_CXXFLAGS) $(CXXLINKS) 

$(T_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	g++ -c $< -o $@ $(T_CXXFLAGS)

