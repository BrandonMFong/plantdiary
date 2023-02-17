/**
 * author: Brando
 * date: 1/6/23
 */

#include "driver.hpp"
#include "database.hpp"
#include "pool.hpp"
#include "pipe.hpp"
#include "logger.hpp"
#include "instructor.hpp"
#include <cpplib.hpp>
#include <common.h>
#include <sys/stat.h>

Driver * gDriver = 0;

Driver::Driver() {

}

Driver::~Driver() {

}

Driver * Driver::shared() {
	return gDriver;
}

int Driver::initialize() {
	int error = 0;
	gDriver = new Driver;

	if ((error = Logger::initialize()) != 0) {
		return error;
	} else if ((error = Database::initialize()) != 0) {
		return error;
	} else if ((error = Pipe::initialize()) != 0) {
		return error;
	} else if ((error = Pool::initialize()) != 0) {
		return error;
	} else if ((gDriver = new Driver) == NULL) {
		return 4;
	} else if ((error = gDriver->setupEnvironment()) != 0) {
		return error;
	} else {
		return 0;
	}
}

void Driver::deinitialize() {
	Pool::deinitialize();
	Pipe::deinitialize();
	Database::deinitialize();

	Delete(gDriver);
	
	return;
}

int Driver::run() {

	Logger::shared()->writeString("Starting loop");

	while (1) {

	}

	return 0;
}

int Driver::setupEnvironment() {
	if (!IsDirectory(kPDCommonAppDataPath)) {
		return mkdir(kPDCommonAppDataPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}

	return 0;
}

typedef struct {
	Instructor * instructor;
} InstructionThreadPackage;

// This funciton will release i memory
void * LaunchInstruction(void * arg) {
	InstructionThreadPackage * p = (InstructionThreadPackage *) arg;
	int error = 0;

	if (p) {
		error = p->instructor->execute();

		if (error != 0)
			Logger::shared()->writeString("Failed: %d", error);

		Delete(p->instructor);
		Free(p);
	}

	return 0;
}

void Driver::executeInstruction(PDInstruction * instructions) {
	int error = 0;
	Instructor * instructor = Instructor::create(instructions);

	if (instructor == NULL) {
		DLog("instructor is null");
	} else {
		// Sweep through the table to see if we have any available threads
		// to launch
		int maxTries = 100;
		int i = 0;
		do {
			bool launched = false;
			for (int j = 0; j < kDriverThreadQueueSize; j++) {
				if (!this->_pTable.active[j]) {
					InstructionThreadPackage * p = (InstructionThreadPackage *) malloc(sizeof(InstructionThreadPackage));
					p->instructor = instructor;
					pthread_create(&this->_pTable.thread[j], 0, LaunchInstruction, p);
					launched = true;
					break;
				}
			}

			if (launched)
				break;
			else
				sleep(1);

			i++;
		} while (i < maxTries);
	}

	Free(instructions);
}

