/**
 * author: Brando
 * date: 1/6/23
 */

#include "pipe.hpp"
#include <common.h>
#include <bflibcpp/bflibcpp.hpp>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "logger.hpp"
#include "driver.hpp"

Pipe * gPipe = 0;
pthread_t pThreadPipeReader;

void * PipeReader(void *) {
	// C program to implement one side of FIFO
	// This side reads first, then reads
    int fd1;

    // FIFO file path
    const char * myfifo = kPDCommonFifoFilePathInbound;

#ifndef TESTING
	BFDLog("Reading %s", myfifo);
#endif

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    while (1) {
		PDInstruction * instr = (PDInstruction *) malloc(sizeof(PDInstruction));

        // First open in read only and read
		// This should be polling
        fd1 = open(myfifo, O_RDONLY);
		size_t expectedSize = sizeof(PDInstruction);
		size_t bytesRead = read(fd1, instr, expectedSize);

		// Driver will own instr mem
		Driver::shared()->executeInstruction(instr);

        // Print the read string and close
        close(fd1);
    }
}

Pipe::Pipe() {

}

Pipe::~Pipe() {

}

int Pipe::initialize() {
	gPipe = new Pipe;

	if (pthread_create(&pThreadPipeReader, NULL, PipeReader, NULL) != 0) {
		return 1;
	}

	PDLog("Pipe initialized");

	return 0;
}

void Pipe::deinitialize() {
	pthread_cancel(pThreadPipeReader);
	Delete(gPipe);
}

Pipe * Pipe::shared() {
	return gPipe;
}

int Pipe::writeResponse(PDResponse * response) {
	int result = 0;
	int f = 0;

	f = open(kPDCommonFifoFilePathOutbound, O_WRONLY);

	if (f == -1) {
		result = 3;
	}

	if (result == 0) {
		BFDLog("Writing response");
		if (write(f, response, sizeof(PDResponse)) == -1) {
			result = 4;
		}

		BFDLog("Finished writing response");
	}

	if (f) close(f);

	return result;
}

