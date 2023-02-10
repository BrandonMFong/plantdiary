/**
 * author: Brando
 * date: 1/26/23
 */

#include "logger.hpp"
#include "delete.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <clib.h>

#define kLoggerFilePath "/tmp/plantdiary.log"

const int kLoggerPrefixSize = 2 << 6;
Logger * gLogger = NULL;

Logger::Logger() {
	this->_descriptor = open(kLoggerFilePath, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	this->_queueSize = 0;

	pthread_mutex_init(&this->_mutexQueue, NULL);

	for (int i = 0; i < kLoggerQueueLengthMax; i++) {
		this->_queue[i] = (char *) malloc(sizeof(char) * (kLoggerPrefixSize + kLoggerLogLength));
	}

#ifdef DEBUG
	if (this->_descriptor == -1) {
		DLog("The descriptor is %d", this->_descriptor);
	}
#endif
}

Logger::~Logger() {
	close(this->_descriptor);

	for (int i = 0; i < kLoggerQueueLengthMax; i++) {
		free(this->_queue[i]);
	}
	pthread_mutex_destroy(&this->_mutexQueue);
}

int Logger::initialize() {
	if ((gLogger = new Logger) == NULL) {
		return 1;
	} else if (pthread_create(&gLogger->_pThreadLogWriter, NULL, Logger::logWriter, gLogger) != 0) {
		DLog("Could not laugh writer thread");
		return 2;
	}

	gLogger->writeString("Log file initialized");

	return 0;
}

void Logger::deinitialize() {
	pthread_cancel(gLogger->_pThreadLogWriter);
	Delete(gLogger);
}

Logger * Logger::shared() {
	return gLogger;
}

void SetLogPrefix(char * str) {
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	struct tm tm = *localtime(&spec.tv_sec);
	
	int h = tm.tm_hour;
	int m = tm.tm_min;
	int s = tm.tm_sec;
	int ms = spec.tv_nsec * pow(10, -6);
	int us = (spec.tv_nsec * pow(10, -3)) - (ms * pow(10, 3));
	int ns = spec.tv_nsec - ((ms * pow(10, 6)) + (us * pow(10, 3)));

	sprintf(str, "%02d/%02d/%d - %02d:%02d:%02d.%03d.%03d.%03d", 
		tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900,
		h, m, s, 
		ms, us, ns
	);
}

void Logger::addToQueue(const char * log) {
	pthread_mutex_lock(&this->_mutexQueue);

	// I will just disregard logging if there is no space in the queue
	// 
	// I shouldn't be writing to the log so many times
	if (this->_queueSize < kLoggerQueueLengthMax) {
		strcpy(this->_queue[this->_queueSize], log);
		this->_queueSize++;
	}

	pthread_mutex_unlock(&this->_mutexQueue);
}

int Logger::queueSize() {
	int result = 0;
	pthread_mutex_lock(&this->_mutexQueue);
	result = this->_queueSize;
	pthread_mutex_unlock(&this->_mutexQueue);
	return result;
}

int Logger::writeString(const char * format, ...) {
	int result = 0;
	va_list args;
	char pref[kLoggerPrefixSize];
	char input[kLoggerLogLength];
	const int size = kLoggerLogLength + kLoggerPrefixSize;
	char buf[size];

	va_start(args, format);

	SetLogPrefix(pref);

	sprintf(input, format, args);
	sprintf(buf, "%s: %s\n", pref, input);

	this->addToQueue(buf);

	va_end(args);

	return result;
}

void * Logger::logWriter(void * obj) {
	Logger * logger = (Logger *) obj;
	int error = 0;

	while (logger != NULL) {
		if (logger->queueSize() > 0) {
			pthread_mutex_lock(&logger->_mutexQueue);

			if (write(logger->_descriptor, logger->_queue[0], strlen(logger->_queue[0]) + 1) == -1) {
				DLog("Could not write '%s' into log file", logger->_queue[0]);
				error = 4;
			} else {
				char * tmp = logger->_queue[0];
				for (int i = 0; i < (kLoggerQueueLengthMax - 1); i++) {
					logger->_queue[i] = logger->_queue[i+1];
				}
				logger->_queue[kLoggerQueueLengthMax - 1] = tmp;

				logger->_queueSize--;
			}

			pthread_mutex_unlock(&logger->_mutexQueue);
		}

		sleep(1);
	}

	return NULL;
}

