/**
 * author: Brando
 * date: 1/26/23
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "accessorspecifiers.hpp"
#include <pthread.h>
#include <log.h>

#ifndef DEBUG
#define PDLog(...) Logger::shared()->writeString(__VA_ARGS__)
#else
#define PDLog(...)
#endif

/**
 * The maximum length of a log entry
 */
const int kLoggerLogLength = 2 << 8;

/**
 * The amount of logs the Logger is allowed to queue up
 */
const int kLoggerQueueLengthMax = 2 << 4;

class Logger {

PUBLIC:
	static int initialize();
	static void deinitialize();

	static Logger * shared();

	/**
	 * Caller does not need to worry about new lines
	 */
	int writeString(const char * format, ...);

	int queueSize();

PRIVATE:
	Logger();
	~Logger();

	/**
	 * Adds log entry to queue
	 */
	void addToQueue(const char * log);

	static void * logWriter(void * logger);

	/**
	 * file descriptor for the log we are writing to
	 */
	int _descriptor;

	/**
	 * holds mutex for locking
	 */
	 pthread_mutex_t _mutexQueue;

	/**
	 * Holds the log entries
	 *
	 * I could've used the queue from my library but I wanted to do the bare minimum
	 */
	char * _queue[kLoggerQueueLengthMax];
	
	/// Holds the size of _queue
	int _queueSize;

	pthread_t _pThreadLogWriter;
};

#endif // LOGGER_HPP

