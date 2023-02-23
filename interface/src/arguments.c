/**
 * author: Brando
 * date: 2/17/23
 */

#include "arguments.h"
#include <string.h>
#include <libgen.h>

int ArgumentsParseArguments(
	int argc,
	char * argv[],
	Arguments * args
) {
	int result = 0;

	if (args == NULL) {
		result = 1;
	} else {
		// Initialize the struct
		args->command = kPDCommandUnknown;
		args->subCommand = -1;
		args->sessionID[0] = '\0';
		args->print = false;
		args->helpPDCommandArg[0] = '\0';
		args->appName[0] = '\0';
	
		// Default command to help
		args->command = kPDCommandHelp;

		for (int i = 0; (i < argc) && (result == 0); i++) {
			if (i == 0) {	
				strcpy(args->appName, basename(argv[i]));
			} else if (i == 1) {
				if (!strcmp(argv[i], kArgumentHelp)) {
					args->command = kPDCommandHelp;
					if ((i + 1) < argc) {
						i++;
						strcpy(args->helpPDCommandArg, argv[i]);
					}
				} else if (!strcmp(argv[i], kArgumentVersion)) {
					args->command = kPDCommandVersion;
					if ((i + 1) < argc) {
						i++;
						if (!strcmp(argv[i], kArgumentVersionDaemon)) {
							args->subCommand = kPDSubCommandVersionGetDaemon;
						} else if (!strcmp(argv[i], kArgumentVersionAll)) {
							args->subCommand = kPDSubCommandVersionGetAll;
						}
					}
				} else if (!strcmp(argv[i], kArgumentGet)) {
					args->command = kPDCommandGet;
				} else if (!strcmp(argv[i], kArgumentSession)) {
					args->command = kPDCommandSession;
					if ((i + 1) < argc) {
						i++;
						if (!strcmp(argv[i], kArgumentSessionStart)) {
							args->subCommand = kPDSubCommandSessionStart;
						} else if (!strcmp(argv[i], kArgumentSessionStatus)) {
							args->subCommand = kPDSubCommandSessionStatus;
						} else if (!strcmp(argv[i], kArgumentSessionStop)) {
							args->subCommand = kPDSubCommandSessionStop;
						}
					} else {
						result = 3;
					}
				} else if (!strcmp(argv[i], kArgumentInit)) {
					args->command = kPDCommandInit;
				}
			} else if (!strcmp(argv[i], kArgumentOtherSessionID)) {
				strcpy(args->sessionID, argv[i + 1]);
			} else if (!strcmp(argv[i], kArgumentOtherPrint)) {
				args->print = true;
			}
		}
	}

	return result;
}

