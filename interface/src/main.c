/**
 * author: Brando
 * date: 1/5/22
 */

#include <stdio.h>
#include <common.h>
#include <libgen.h>
#include <bflibc/bflibc.h>
#include <string.h>
#include "interface.h"

// Argument sources
#include "arguments.h"
#include "version.h"
#include "get.h"
#include "set.h"
#include "help.h"
#include "session.h"
#include "init.h"

int main(int argc, char ** argv) {
	int error = 0;
	char upath[PATH_MAX];
	bool okayToContinue = true;
	Arguments args;

	error = ArgumentsParseArguments(argc, argv, &args);

	if (error == 0) {
		error = InterfaceGetUserDataPath(upath);
	}

	if (error == 0) {
		switch (args.command) {
		case kPDCommandVersion:
			error = VersionExec(&args);
			break;
		case kPDCommandGet:
			error = GetExec(&args);
			break;
		case kPDCommandSet:
			error = SetExec(&args);
			break;
		case kPDCommandSession:
			error = SessionExec(&args);
			break;
		case kPDCommandInit:
			error = InitExec(&args);
			break;
		case kPDCommandHelp:
			error = HelpExec(&args);
			break;
		default:
			break;
		}
	}

    return error;
}

