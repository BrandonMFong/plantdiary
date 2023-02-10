# author: Brando
# date: 7/29/22
#

.PHONY: all common clean release debug

release:
	mkdir -p bin
	cd backend && make release
	cd interface && make release
	cp -afv backend/bin/* bin/
	cp -afv interface/bin/* bin/

all: clean common release debug

common:
	cd common && make clean
	cd common && make

clean:
	cd backend && make clean
	cd interface && make clean
	rm -rfv bin/

debug:
	cd backend && make debug
	cd interface && make debug

