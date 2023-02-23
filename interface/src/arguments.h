/**
 * author: Brando
 * date: 2/1/23
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "typearguments.h"

/// commands
#define kArgumentHelp "help"
#define kArgumentVersion "version"
#define kArgumentGet "get"
#define kArgumentSession "session"
#define kArgumentInit "init"

/// subcommands
#define kArgumentGetPlantCount "plantcount"

#define kArgumentSessionStart "start"
#define kArgumentSessionStatus "status"
#define kArgumentSessionStop "stop"

#define kArgumentVersionDaemon "--daemon"
#define kArgumentVersionAll "--all"

/// other arguments
#define kArgumentOtherSessionID "-s"
#define kArgumentOtherPrint "-p"

/**
 * Parses command line arguments
 */
int ArgumentsParseArguments(
	int argc,
	char * argv[],
	Arguments * args
);

#endif // ARGUMENTS_H

