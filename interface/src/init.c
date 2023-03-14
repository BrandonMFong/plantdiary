/**
 * author: Brando
 * date: 2/16/23
 */

#include "init.h"
#include "arguments.h"
#include <stdio.h>
#include "interface.h"
#include "print.h"
#include <bflibc/bflibc.h>
#include <stdlib.h>
#include <sys/stat.h>

void InitHelp() {
	printf("Argument : %s\n", kArgumentInit);
	printf("  Defines a '%s' directory in user's home directory\n");
}

int InitExec(Arguments * args) {
	char path[PATH_MAX];
	int result = InterfaceGetUserDataPath(path);

	if (result == 0) {
		if (BFFileSystemPathIsDirectory(path)) {
			print("Already initialized\n");
		} else {
			if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
				BFErrorPrint("Could not make path: %s", path);
				result = 30;
			}

			if (result == 0) {
				print("Successfully initialized\n");
			}
		}
	}

	return result;
}

