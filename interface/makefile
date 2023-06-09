# author: Brando
# date: 7/29/22
#

include ../common/external/libs/makefiles/libpaths.mk

### Global
BUILD_PATH = build
FILES = fifo version help get session init interface arguments set
CLINKS = -lm -luuid

### Release settings
R_CFLAGS += -I. -I../common/bin/bflib/release/ -I../common
R_BIN_NAME = plantdiary
R_BUILD_PATH = $(BUILD_PATH)/release
R_MAIN_FILE = src/main.c
R_LIBRARIES = ../common/bin/bflib/release/bflibc/libbfc.a ../common/bin/json.a
R_OBJECTS = $(patsubst %, $(R_BUILD_PATH)/%.o, $(FILES))

### Debug settings
D_CFLAGS = -DDEBUG -g -I. -I../common/bin/bflib/debug/ -I../common
D_BIN_NAME = $(R_BIN_NAME)-debug
D_BUILD_PATH = $(BUILD_PATH)/debug
D_MAIN_FILE = $(R_MAIN_FILE)
D_LIBRARIES = ../common/bin/bflib/release/bflibc/libbfc.a ../common/bin/json.a
D_OBJECTS = $(patsubst %, $(D_BUILD_PATH)/%.o, $(FILES))

### Test settings
T_CFLAGS = $(D_CFLAGS) -DTESTING -Isrc/ -I../common/external/libs/bflibc/testbench
T_BIN_NAME = $(R_BIN_NAME)-test
T_BUILD_PATH = $(BUILD_PATH)/test
T_MAIN_FILE = testbench/tests.c
T_LIBRARIES = $(D_LIBRARIES)
T_OBJECTS = $(patsubst %, $(T_BUILD_PATH)/%.o, $(FILES))

### Instructions

# Default
all: release

libraries:
	cd ../common/external/libs && make clean
	cd ../common/external/libs && make release
	cd ../common/external/libs && make debug 

clean:
	rm -rfv build
	rm -rfv bin

## Release build instructions
release: release-setup bin/$(R_BIN_NAME)

release-setup:
	@mkdir -p $(R_BUILD_PATH)
	@mkdir -p bin

bin/$(R_BIN_NAME): $(R_MAIN_FILE) $(R_OBJECTS) $(R_LIBRARIES)
	gcc -o $@ $^ $(R_CFLAGS) $(CLINKS)

$(R_BUILD_PATH)/%.o: src/%.c src/%.h
	gcc -c $< -o $@ $(R_CFLAGS)

## Debug build instructions
debug: debug-setup bin/$(D_BIN_NAME)

debug-setup:
	@mkdir -p $(D_BUILD_PATH)
	@mkdir -p bin

bin/$(D_BIN_NAME): $(D_MAIN_FILE) $(D_OBJECTS) $(D_LIBRARIES)
	gcc -o $@ $^ $(D_CFLAGS) $(CLINKS)

$(D_BUILD_PATH)/%.o: src/%.c src/%.h
	gcc -c $< -o $@ $(D_CFLAGS)

## Test build instructions
test: test-setup bin/$(T_BIN_NAME)
	./bin/$(T_BIN_NAME)

test-setup:
	@mkdir -p $(T_BUILD_PATH)
	@mkdir -p bin

bin/$(T_BIN_NAME): $(T_MAIN_FILE) $(T_OBJECTS) $(T_LIBRARIES)
	gcc -o $@ $^ $(T_CFLAGS) $(CLINKS)

$(T_BUILD_PATH)/%.o: src/%.c src/%.h
	gcc -c $< -o $@ $(T_CFLAGS)

