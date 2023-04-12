# author: Brando
# date: 2/1/23
#

include external/libs/makefiles/libpaths.mk

### Global
BUILD_PATH = build
FILES = species
CLINKS =

COMMON_R_CFLAGS = -I. -I./external/libs/$(BF_LIB_RPATH_RELEASE) 
COMMON_D_CFLAGS = $(COMMON_R_CFLAGS) -DDEBUG -g
COMMON_R_BUILD_PATH = $(BUILD_PATH)/release
COMMON_D_BUILD_PATH = $(BUILD_PATH)/debug
COMMON_R_OBJECTS = $(patsubst %, $(COMMON_R_BUILD_PATH)/%.o, $(FILES))
COMMON_D_OBJECTS = $(patsubst %, $(COMMON_D_BUILD_PATH)/%.o, $(FILES))

all: mysql-connector-cpp libs json-parser common

setup:
	mkdir -p bin
	mkdir -p bin/bflib
	mkdir -p build
	mkdir -p build/release
	mkdir -p build/debug
	cd external/mysql-connector-cpp && sudo cmake .

clean:
	rm -rfv bin
	rm -rfv build
	cd external/libs && make clean

clean-mysql:
	cd external/mysql-connector-cpp && sudo make clean

mysql-connector-cpp:
	cd external/mysql-connector-cpp && sudo make
	cd external/mysql-connector-cpp && sudo make install

libs:
	cd external/libs && make
	cp -afv external/libs/bin/* bin/bflib

json-parser:
	gcc -c -o build/json.o external/json-parser/json.c
	ar -rsc bin/json.a build/json.o

## Release build instructions
common: $(COMMON_R_OBJECTS) $(COMMON_D_OBJECTS)
	ar -rsc bin/$@.a $(COMMON_R_OBJECTS)
	ar -rsc bin/$@-debug.a $(COMMON_D_OBJECTS)

$(COMMON_R_BUILD_PATH)/%.o: ./internal/%.c ./internal/%.h
	gcc -c $< -o $@ $(COMMON_R_CFLAGS)

$(COMMON_D_BUILD_PATH)/%.o: ./internal/%.c ./internal/%.h
	gcc -c $< -o $@ $(COMMON_D_CFLAGS)

