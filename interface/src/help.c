/**
 * author: Brando
 * date:
 */

#include "help.h"
#include "arguments.h"
#include "get.h"
#include "session.h"
#include "init.h"
#include <stdio.h>
#include <string.h>

void help(Arguments * args) {
	printf("usage: %s <commands> [ %s <session id> ]\n", args->appName, kArgumentOtherSessionID);
	printf("\n");

	printf("Commands:\n");
	printf("  %s : Initializes the user's environment for use\n", kArgumentInit);
	printf("  %s : Returns version for cli and gackground tool\n", kArgumentVersion);
	printf("  %s : Provides more details on commands\n", kArgumentHelp);
	printf("  %s : Returns data based on subcommand\n", kArgumentGet);
	printf("  %s : Assists session status\n", kArgumentSession);

	printf("\n");

	printf("Other:\n");
	printf("  %s <session id> : Session id is used to uniquely identify you as the user when using this cli\n", kArgumentOtherSessionID);

	printf("\n");

	printf("Plant Diary copyright %s. All rights reserved.\n", &__DATE__[7]);
}

int HelpExec(Arguments * args) {
	if (strlen(args->helpPDCommandArg) == 0) {
		help(args);
	} else {
		if (!strcmp(args->helpPDCommandArg, kArgumentGet)) {
			GetHelp();
		} else if (!strcmp(args->helpPDCommandArg, kArgumentSession)) {
			SessionHelp();
		} else if (!strcmp(args->helpPDCommandArg, kArgumentInit)) {
			InitHelp();
		} else {
			printf("Unknown command: %s\n", args->helpPDCommandArg);
		}
	}

	return 0;
}
