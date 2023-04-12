/**
 * author: Brando
 * date: 2/17/23
 */

#include "arguments.h"
#include <string.h>
#include <libgen.h>
#include <common.h>

int ArgumentsParseArgumentsForSession(int, int, char **, Arguments *);
int ArgumentsParseArgumentsForSet(int, int, char **, Arguments *);

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
		args->subCommand = kPDSubCommandUnkown;
		args->sessionID[0] = '\0';
		args->type.session.print = false;
		args->type.help.helpPDCommandArg[0] = '\0';
		args->appName[0] = '\0';
		args->type.set.plant.option = kPDSetPlantOptionNone;
	
		// Default command to help
		args->command = kPDCommandHelp;

		for (int i = 0; (i < argc) && (result == 0); i++) {
			if (i == 0) {	
				strcpy(args->appName, basename(argv[i]));

			// Main commands
			} else if (i == 1) {
				// Help
				if (!strcmp(argv[i], kArgumentHelp)) {
					args->command = kPDCommandHelp;
					if ((i + 1) < argc) {
						i++;
						strcpy(args->type.help.helpPDCommandArg, argv[i]);
					}

				// Version
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

				// Get
				} else if (!strcmp(argv[i], kArgumentGet)) {
					args->command = kPDCommandGet;
				
				// Set
				} else if (!strcmp(argv[i], kArgumentSet)) {
					args->command = kPDCommandSet;
					if ((i + 1) < argc) {
						i++;
						if (!strcmp(argv[i], kArgumentSetEvent)) {
							args->subCommand = kPDSubCommandSetEvent;
						} else if (!strcmp(argv[i], kArgumentSetPlant)) {
							args->subCommand = kPDSubCommandSetPlant;
						}

						result = ArgumentsParseArgumentsForSet(i, argc, argv, args);
					}

				// Session
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

						result = ArgumentsParseArgumentsForSession(i, argc, argv, args);
					} else {
						result = 3;
					}

				// Init
				} else if (!strcmp(argv[i], kArgumentInit)) {
					args->command = kPDCommandInit;
				}
			} else if (!strcmp(argv[i], kArgumentOtherSessionID)) {
				strcpy(args->sessionID, argv[i + 1]);
			}
		}
	}

	return result;
}

int ArgumentsParseArgumentsForSession(
	int startIndex,
	int argc,
	char * argv[],
	Arguments * args
) {
	int result = 0;
	for (int i = startIndex; (i < argc) && (result == 0); i++) {
		if (!strcmp(argv[i], kArgumentSessionIDPrint)) {
			args->type.session.print = true;
		}
	}

	return result;;
}

int ArgumentsParseArgumentsForSet(
	int startIndex,
	int argc,
	char * argv[],
	Arguments * args
) {
	int result = 0;
	for (int i = startIndex; (i < argc) && (result == 0); i++) {
		if (!strcmp(argv[i], kArgumentSetEventType)) {
			if ((i + 1) < argc) {
				i++;
				if (!strcmp(argv[i], kArgumentSetEventTypeWater)) {
					strcpy(args->type.set.event.type, kPDSetEventTypePlantWater);
				}
			}
		} else if (!strcmp(argv[i], kArgumentSetPlantNew)) {
			args->type.set.plant.option = kPDSetPlantOptionNew;
		} else if (!strcmp(argv[i], kArgumentSetPlantName)) {
			if ((i + 1) < argc) {
				i++;
				strncpy(args->type.set.plant.name, argv[i], kPDCommonPlantNameStringLength);
			}
		} else if (!strcmp(argv[i], kArgumentSetPlantSpecies)) {
			if ((i + 1) < argc) {
				i++;
				strncpy(args->type.set.plant.species, argv[i], kPDCommonPlantSpeciesStringLength);
			}
		} else if (!strcmp(argv[i], kArgumentSetPlantUUID)) {
			if ((i + 1) < argc) {
				i++;
				strncpy(args->type.set.plant.uuid, argv[i], kBFStringUUIDStringLength);
			}
		}
	}

	return result;;
}

