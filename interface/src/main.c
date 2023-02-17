/**
 * author: Brando
 * date: 1/5/22
 */

#include <stdio.h>
#include <common.h>
#include <libgen.h>
#include <clib.h>
#include <string.h>
#include "interface.h"

// Argument sources
#include "arguments.h"
#include "version.h"
#include "get.h"
#include "help.h"
#include "session.h"
#include "init.h"

void help(int argc, char ** argv) {
	printf("usage: %s <commands> [ %s <session id> ]\n", basename(argv[0]), kArgumentOtherSessionID);
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

int main(int argc, char ** argv) {
	int error = 0;
	char upath[PATH_MAX];
	bool okayToContinue = true;

	if (argc == 1) {
		help(argc, argv);
		okayToContinue = false;
	} else if (!strcmp(argv[1], kArgumentVersion)) {
		error = VersionExec(argc, argv);
		okayToContinue = false;
	} else if (!strcmp(argv[1], kArgumentInit)) {
		error = InitExec(argc, argv);
		okayToContinue = false;
	} else {
		error = InterfaceGetUserDataPath(upath);
	}

	if (okayToContinue && (error == 0)) {
		// Make sure we have a place to store user data.
		//
		// This way we can safely assume we have a place to write data to
		if (!IsDirectory(upath)) {
			Error("User environment was not initialize. Please do '%s'", kArgumentInit);
			error = -1;
		} else {
			if (!strcmp(argv[1], kArgumentHelp)) {
				error = HelpExec(argc, argv);
			} else if (!strcmp(argv[1], kArgumentGet)) {
				error = GetExec(argc, argv);
			} else if (!strcmp(argv[1], kArgumentSession)) {
				error = SessionExec(argc, argv);
			} else {
				Error("unknown argument, '%s'", argv[1]);
				error = -1;
			}
		}
	}

    return error;
}

