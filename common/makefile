# author: Brando
# date: 2/1/23
#

all: setup mysql-connector-cpp libs json-parser

setup:
	mkdir -p bin
	mkdir -p build

clean:
	rm -rfv bin
	rm -rfv build

mysql-connector-cpp:
	cd external/mysql-connector-cpp && sudo cmake .
	cd external/mysql-connector-cpp && sudo make clean
	cd external/mysql-connector-cpp && sudo make
	cd external/mysql-connector-cpp && sudo make install

libs:
	cd external/libs && make
	cd external/libs && make debug
	cd external/libs && make test
	cp -afv external/libs/bin/* bin

json-parser:
	gcc -c -o build/json.o external/json-parser/json.c
	ar -rsc bin/json.a build/json.o

