# author: Brando
# date: 2/1/23
#

all: setup mysql-connector-cpp libs json-parser common

setup:
	mkdir -p bin
	mkdir -p build
	mkdir -p build/release
	mkdir -p build/debug

clean:
	rm -rfv bin
	rm -rfv build
	cd external/libs && make clean
	cd external/mysql-connector-cpp && sudo make clean

mysql-connector-cpp:
	cd external/mysql-connector-cpp && sudo cmake .
	cd external/mysql-connector-cpp && sudo make
	cd external/mysql-connector-cpp && sudo make install

libs:
	cd external/libs && make
	cd external/libs && make debug
	cp -afv external/libs/bin/* bin

json-parser:
	gcc -c -o build/json.o external/json-parser/json.c
	ar -rsc bin/json.a build/json.o

### Global
BUILD_PATH = build
FILES = common
CLINKS =

COMMON_R_CFLAGS = -I. -I./external/libs/clib/ 
COMMON_D_CFLAGS = $(COMMON_R_CFLAGS) -DDEBUG -g
COMMON_R_BUILD_PATH = $(BUILD_PATH)/release
COMMON_D_BUILD_PATH = $(BUILD_PATH)/debug
COMMON_R_OBJECTS = $(patsubst %, $(COMMON_R_BUILD_PATH)/%.o, $(FILES))
COMMON_D_OBJECTS = $(patsubst %, $(COMMON_D_BUILD_PATH)/%.o, $(FILES))

## Release build instructions
common: $(COMMON_R_OBJECTS) $(COMMON_D_OBJECTS)
	ar -rsc bin/$@.a $(COMMON_R_OBJECTS)
	ar -rsc bin/$@-debug.a $(COMMON_D_OBJECTS)

$(COMMON_R_BUILD_PATH)/%.o: ./%.c ./%.h
	gcc -c $< -o $@ $(COMMON_R_CFLAGS)

$(COMMON_D_BUILD_PATH)/%.o: ./%.c ./%.h
	gcc -c $< -o $@ $(COMMON_D_CFLAGS)

