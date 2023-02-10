/**
 * author: Brando
 * date:
 */

#include "help.h"
#include "arguments.h"
#include "get.h"
#include "session.h"
#include <stdio.h>
#include <string.h>

int HelpExec(int argc, char ** argv) {
	if (argc == 2) {
		printf("Please provide an argument to the %s command\n", kArgumentHelp);
	} else {
		if (!strcmp(argv[2], kArgumentGet)) {
			GetHelp();
		} else if (!strcmp(argv[2], kArgumentSession)) {
			SessionHelp();
		} else {
			printf("Unknown command: %s\n", argv[2]);
		}
	}

	return 0;
}
