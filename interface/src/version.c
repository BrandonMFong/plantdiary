/**
 * author: Brando
 * date: 2/1/23
 */

#include "version.h"
#include "fifo.h"
#include <stddef.h>
#include <stdio.h>
#include <common.h>
#include <string.h>
#include <clib.h>
#include "meta.h"
#include "print.h"

int VersionExec(int argc, char ** argv) {
	PDInstruction instr = {0};
	PDResponse resp = {0};
	PDInstructionSetCommand(&instr, kPDCommandVersion);
	int error = FifoWrite(&instr);

	if (error == 0) {
		error = FifoRead(&resp);
	}

	if (error == 0) {
		print("plantdiary version: %s\n", resp.data);
		print("plantdiary-cli version: %s\n", kVersionString);
	}

	return error;
}

