# author: Brando
# date: 7/29/22
#

.PHONY: clean release debug

all: clean setup release install

release: bin
	cd common && make
	cd backend && make release
	cd interface && make release
	cp -afv backend/bin/* bin/
	cp -afv backend/config/plantdiaryd.service bin/
	cp -afv interface/bin/* bin/

install:
	sudo cp -afv bin/plantdiaryd /usr/sbin
	sudo cp -afv bin/plantdiaryd.service /etc/systemd/system
	sudo systemctl daemon-reload
	sudo cp -afv bin/plantdiary /usr/local/bin

clean:
	cd common && make clean
	cd backend && make clean
	cd interface && make clean
	rm -rfv bin/

debug: bin
	cd common && make
	cd backend && make debug
	cd interface && make debug
	cp -afv backend/bin/* bin/
	cp -afv interface/bin/* bin/

bin:
	mkdir -p bin

setup:
	cd common && make setup

