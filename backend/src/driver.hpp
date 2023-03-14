/**
 * author: Brando
 * date: 1/6/23
 */

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <bflibcpp/accessorspecifiers.hpp>
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

	/**
	 * Sets up the directory where our app data will leave
	 */
	int setupEnvironment();

	/**
	 * Holds a list of threads and another list of active flags that 
	 * correspond to the thread at the index
	 *
	 * The mutex array should only be used on the threads and active flags
	 * at the same index
	 *
	 */
	struct {
		pthread_t thread[kDriverThreadQueueSize];
		bool active[kDriverThreadQueueSize];
		pthread_mutex_t mutex[kDriverThreadQueueSize];
	} _pTable;
};

#endif // DRIVER_HPP

