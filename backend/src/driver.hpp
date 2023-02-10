/**
 * author: Brando
 * date: 1/6/23
 */

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "accessorspecifiers.hpp"
#include <internal/instructions.h>
#include <pthread.h>

#define kDriverThreadQueueSize 10

class Driver {
PUBLIC:
	static int initialize();
	static void deinitialize();

	static int run();
	static Driver * shared();

	/**
	 * param instructions: caller must malloc the memory.  We will release
	 */
	void executeInstruction(PDInstruction * instructions);
	
PRIVATE:
	Driver();
	~Driver();

	struct {
		pthread_t thread[kDriverThreadQueueSize];
		bool active[kDriverThreadQueueSize];
	} _pTable;
};

#endif // DRIVER_HPP

