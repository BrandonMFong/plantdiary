/**
 * author: Brando
 * date: 1/6/23
 */

#ifndef PIPE_HPP
#define PIPE_HPP

#include "accessorspecifiers.hpp"
#include <internal/responses.h>

class Pipe {
PUBLIC:
	static int initialize();
	static void deinitialize();
	static Pipe * shared();

	int writeResponse(PDResponse * response);

PRIVATE:
	Pipe();
	~Pipe();
};

#endif // PIPE_HPP

