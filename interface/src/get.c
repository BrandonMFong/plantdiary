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
#include <bflibc/log.h>

/// Sub commands functions
int GetPlantCount();

void GetHelp() {
	printf("Argument : %s\n", kArgumentGet);
	printf("  sub commands\n");
	printf("    - %s : returns the number of plants for user\n", kArgumentGetPlantCount);
	printf("    - %s : returns a list of plants for user\n", kArgumentGetPlantCount);
}

int GetExec(Arguments * args) {
	PDInstruction i = {0};
	PDResponse resp = {0};
	PDInstructionSetCommand(&i, kPDCommandGet);

	if (args->subCommand == -1) {
		printf("Please provide a subcommand for %s\n", kArgumentGet);
	} else {
		if (args->subCommand == kPDSubCommandGetPlantCount) {
			PDInstructionSetSubCommand(&i, kPDSubCommandGetPlantCount);
		} else if (args->subCommand == kPDSubCommandGetPlantList) {
			BFDLog("Listing plants");
			PDInstructionSetSubCommand(&i, kPDSubCommandGetPlantList);
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

