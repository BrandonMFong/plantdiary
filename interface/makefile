# author: Brando
# date: 7/29/22
#

### Global
BUILD_PATH = build
FILES = fifo version help get session
CLINKS = -lm

### Release settings
R_CFLAGS += -I. -I../common/bin/c/ -I../common
R_BIN_NAME = plantdiary
R_BUILD_PATH = $(BUILD_PATH)/release
R_MAIN_FILE = src/main.c
R_LIBRARIES = ../common/bin/c/clib.a ../common/bin/json.a
R_OBJECTS = $(patsubst %, $(R_BUILD_PATH)/%.o, $(FILES))

### Debug settings
D_CFLAGS = $(R_CFLAGS) -DDEBUG -g
D_BIN_NAME = debug-$(R_BIN_NAME)
D_BUILD_PATH = $(BUILD_PATH)/debug
D_MAIN_FILE = $(R_MAIN_FILE)
D_LIBRARIES = ../common/bin/c/debug-clib.a ../common/bin/json.a
D_OBJECTS = $(patsubst %, $(D_BUILD_PATH)/%.o, $(FILES))

### Test settings
T_CFLAGS = $(R_CFLAGS) -g -DDEBUG -DTESTING -Isrc/ -I../common/external/libs/clib/testbench
T_BIN_NAME = test-$(R_BIN_NAME)
T_BUILD_PATH = $(BUILD_PATH)/test
T_MAIN_FILE = src/testbench/tests.c
T_LIBRARIES = ../common/bin/c/debug-clib.a ../common/bin/json.a
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

