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
#include <bflibcpp/bflibcpp.hpp>
#include <common.h>
#include <sys/stat.h>

Driver * gDriver = 0;

Driver::Driver() {
	for (int i = 0; i < kDriverThreadQueueSize; i++) {
		BFDLog("Initialzing queue %d", i + 1);
		this->_pTable.active[i] = false;
		pthread_mutex_init(&this->_pTable.mutex[i], NULL);
	}
}

Driver::~Driver() {
	for (int i = 0; i < kDriverThreadQueueSize; i++) {
		pthread_mutex_destroy(&this->_pTable.mutex[i]);
	}
}

Driver * Driver::shared() {
	return gDriver;
}

int Driver::initialize() {
	int error = 0;

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
	if (!BFFileSystemPathIsDirectory(kPDCommonAppDataPath)) {
		return mkdir(kPDCommonAppDataPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}

	return 0;
}

typedef struct {
	Instructor * instructor;

	bool * activeFlag;
	pthread_mutex_t * activeMutex;
} InstructionThreadPackage;

// This funciton will release i memory
void * LaunchInstruction(void * arg) {
	InstructionThreadPackage * p = (InstructionThreadPackage *) arg;
	int error = 0;

	if (p) {
		BFDLog("Instructor will now execute its task");
		error = p->instructor->execute();

		if (error != 0)
			Logger::shared()->writeString("Failed: %d", error);

		// Atomically set the flag so that driver knows
		// this queue is available now
		BFDLog("Locking active mutex");
		pthread_mutex_lock(p->activeMutex);
		BFDLog("Releasing queue");
		*p->activeFlag = false;
		pthread_mutex_unlock(p->activeMutex);

		Delete(p->instructor);
		BFFree(p);
	}

	return 0;
}

void Driver::executeInstruction(PDInstruction * instructions) {
	int error = 0;
	Instructor * instructor = Instructor::create(instructions);

	if (instructor == NULL) {
		BFDLog("instructor is null");
	} else {
		// Sweep through the table to see if we have any available threads
		// to launch
		int maxTries = 100;
		int i = 0;
		do {
			bool launched = false;

			// Find available threads
			for (int j = 0; (j < kDriverThreadQueueSize) && !launched; j++) {
				pthread_mutex_lock(&this->_pTable.mutex[i]);
				// If not active, then we will use this thread
				if (!this->_pTable.active[j]) {
					// Create the struct that will hold all necessary items to launch instructor
					InstructionThreadPackage * p = (InstructionThreadPackage *) malloc(sizeof(InstructionThreadPackage));
					p->instructor = instructor;
					p->activeFlag = &this->_pTable.active[j];
					p->activeMutex = &this->_pTable.mutex[j];

					// Launch the thread
					pthread_create(&this->_pTable.thread[j], 0, LaunchInstruction, p);
					launched = true;
				}
				pthread_mutex_unlock(&this->_pTable.mutex[i]);
			}

			if (launched) break;
			else sleep(1); // We will wait a second until we look for another thread

			i++;
		} while (i < maxTries);
	}

	BFFree(instructions);
}

