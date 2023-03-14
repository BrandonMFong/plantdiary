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
#include <bflibc/bflibc.h>
#include "meta.h"
#include "print.h"
#include "arguments.h"

void VersionHelp() {
	printf("Argument : %s\n", kArgumentVersion);
	printf("  sub commands\n");
	printf("    - %s : returns the version for plantdiaryd\n", kArgumentVersionDaemon);
	printf("    - %s : returns version for both this app and plantdiaryd\n", kArgumentVersionAll);
}

int VersionExec(Arguments * args) {
	PDInstruction instr = {0};
	PDResponse resp = {0};
	int error = 0;
	char daemonVersion[32];

	daemonVersion[0] = '\0';

	if ((args->subCommand == kPDSubCommandVersionGetDaemon) || (args->subCommand == kPDSubCommandVersionGetAll)) {
		PDInstructionSetCommand(&instr, kPDCommandVersion);
		error = FifoWrite(&instr);
		BFDLog("%d", error);

		if (error == 0) {
			error = FifoRead(&resp);
			BFDLog("%d", error);
		}

		if (error == 0) {
			strcpy(daemonVersion, resp.data);
		}
	}

	if (error == 0) {
		if (args->subCommand != kPDSubCommandVersionGetDaemon) {
			print("plantdiary version: %s\n", kVersionString);
		}

		if (strlen(daemonVersion) > 0) {
			print("plantdiaryd version: %s\n", resp.data);
		}

	}

	return error;
}

