/**
 * author: Brando
 * date: 2/1/23
 */

#include "fifo.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <common.h>
#include <clib.h>

int FifoWrite(PDInstruction * instr) {
	int result = 0;
    int fd = 0;
    const char * myfifo = kPDCommonFifoFilePathInbound;

#ifndef TESTING

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    if (mkfifo(myfifo, 0666) == -1) {
		result = 30;
	}

	// Open FIFO for write only
	if (result == 0) {
		fd = open(myfifo, O_WRONLY);
		if (fd == -1) {
			result = 31;
		}
	}

	// Write the input arr2ing on FIFO
	// and close it
	if (result == 0) {	
		if (write(fd, instr, sizeof(PDInstruction)) == -1) {
			result = 32;
		}
	}

	close(fd);
#endif

	return result;
}

int FifoRead(PDResponse * response) {
	if (response == NULL) {
		return 33;
	} else {
		int result = 0;
		int d = 0;
#ifndef TESTING
		if (mkfifo(kPDCommonFifoFilePathOutbound, 0666) == -1) {
			result = 34;
		}

		if (result == 0) {
			d = open(kPDCommonFifoFilePathOutbound, O_RDONLY);
			if (d == -1) {
				result = 35;
			}
		}

		if (result == 0) {
			if (read(d, response, sizeof(PDResponse)) == -1) {
				result = 36;
			}
		}

		close(d);
#endif
		return 0;
	}
}

