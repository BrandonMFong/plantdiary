/**
 * author: Brando
 * date:
 */

#include "help.h"
#include "arguments.h"
#include "get.h"
#include "set.h"
#include "session.h"
#include "init.h"
#include "version.h"
#include <stdio.h>
#include <string.h>

void help(Arguments * args) {
	printf("usage: %s <commands> [ <sub commands> ] [ %s <session id> ]\n", args->appName, kArgumentOtherSessionID);
	printf("\n");

	printf("Commands:\n");
	printf("  %s : Initializes the user's environment for use\n", kArgumentInit);
	printf("  %s : Returns version for cli and gackground tool\n", kArgumentVersion);
	printf("  %s : Returns data based on subcommand\n", kArgumentGet);
	printf("  %s : Creates/Modifies data\n", kArgumentSet);
	printf("  %s : Assists session status\n", kArgumentSession);
	printf("  %s : Provides more details on commands\n", kArgumentHelp);

	printf("\n");

	printf("Other:\n");
	printf("  %s <session id> : Session id is used to uniquely identify you as the user when using this cli\n", kArgumentOtherSessionID);

	printf("\n");

	printf("Plant Diary copyright %s. All rights reserved.\n", &__DATE__[7]);
}

int HelpExec(Arguments * args) {
	if (strlen(args->type.help.helpPDCommandArg) == 0) {
		help(args);
	} else {
		if (!strcmp(args->type.help.helpPDCommandArg, kArgumentGet)) {
			GetHelp();
		} else if (!strcmp(args->type.help.helpPDCommandArg, kArgumentSession)) {
			SessionHelp();
		} else if (!strcmp(args->type.help.helpPDCommandArg, kArgumentInit)) {
			InitHelp();
		} else if (!strcmp(args->type.help.helpPDCommandArg, kArgumentVersion)) {
			VersionHelp();
		} else if (!strcmp(args->type.help.helpPDCommandArg, kArgumentSet)) {
			SetHelp();
		} else {
			printf("Unknown command: %s\n", args->type.help.helpPDCommandArg);
		}
	}

	return 0;
}
