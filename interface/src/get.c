/**
 * author: Brando
 * date: 2/3/23
 */

#include "get.h"
#include "fifo.h"
#include "arguments.h"
#include <internal/instructions.h>
#include <stdio.h>
#include <string.h>

/// Sub commands functions
int GetPlantCount();

void GetHelp() {
	printf("Argument : %s\n", kArgumentGet);
	printf("  sub commands\n");
	printf("    - %s : returns the number of plants for user\n", kArgumentGetPlantCount);
}

int GetExec(int argc, char ** argv) {
	PDInstruction i = {0};
	PDResponse resp = {0};
	PDInstructionSetCommand(&i, kPDCommandGet);

	if (argc < 3) {
		printf("Please provide a subcommand for %s\n", kArgumentGet);
	} else {
		if (!strcmp(argv[2], kArgumentGetPlantCount)) {
			PDInstructionSetSubCommand(&i, kPDSubCommandGetPlantCount);
		} else {
			printf("unknown command\n");
		}
	}

	int error = FifoWrite(&i);

	if (error == 0) {
		error = FifoRead(&resp);
	}

	if (error == 0) {
		printf("plant count: %s\n", resp.data);
	}

	return 0;
}

