/**
 * author: Brando
 * date: 1/5/22
 */

#include <stdio.h>
#include <common.h>
#include <libgen.h>
#include <clib.h>
#include "arguments.h"
#include "version.h"
#include "get.h"
#include "help.h"
#include "session.h"
#include <string.h>

void help(int argc, char ** argv) {
	printf("usage: %s <commands> [ %s <session id> ]\n", basename(argv[0]), kArgumentSessionID);
	printf("\n");

	printf("Commands:\n");
	printf("  %s : Returns version for cli and gackground tool\n", kArgumentVersion);
	printf("  %s : Provides more details on commands\n", kArgumentHelp);
	printf("  %s : Returns data based on subcommand\n", kArgumentGet);
	printf("  %s : Assists session status\n", kArgumentSession);

	printf("\n");

	printf("Other:\n");
	printf("  %s <session id> : Session id is used to uniquely identify you as the user when using this cli\n", kArgumentSessionID);

	printf("\n");

	printf("Plant Diary copyright %s. All rights reserved.\n", &__DATE__[7]);
}

int main(int argc, char ** argv) {
	if (argc == 1) {
		help(argc, argv);
	} else {
		if (!strcmp(argv[1], kArgumentVersion)) {
			return VersionExec(argc, argv);
		} else if (!strcmp(argv[1], kArgumentHelp)) {
			return HelpExec(argc, argv);
		} else if (!strcmp(argv[1], kArgumentGet)) {
			return GetExec(argc, argv);
		} else if (!strcmp(argv[1], kArgumentSession)) {
			return SessionExec(argc, argv);
		} else {
			return -1;
		}
	}

    return 0;
}

