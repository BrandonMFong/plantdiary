/**
 * author: Brando
 * date: 2/1/23
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "typearguments.h"
#include <internal/setevents.h>

/// commands
#define kArgumentHelp "help"
#define kArgumentVersion "version"
#define kArgumentGet "get"
#define kArgumentSession "session"
#define kArgumentInit "init"
#define kArgumentSet "set"

/// subcommands

// Get
#define kArgumentGetPlantCount "plantcount"

// Set
#define kArgumentSetEvent "event"
#define kArgumentSetEventType "-type"
#define kArgumentSetEventTypeWater "water"
#define kArgumentSetEventTypeRepot "repot"
#define kArgumentSetPlant "plant"
#define kArgumentSetPlantNew "--new"

// Session
#define kArgumentSessionStart "start"
#define kArgumentSessionStatus "status"
#define kArgumentSessionStop "stop"
#define kArgumentSessionIDPrint "-p"

// Version
#define kArgumentVersionDaemon "--daemon"
#define kArgumentVersionAll "--all"

/// other arguments
#define kArgumentOtherSessionID "-s"

/**
 * Parses command line arguments
 */
int ArgumentsParseArguments(
	int argc,
	char * argv[],
	Arguments * args
);

#endif // ARGUMENTS_H

