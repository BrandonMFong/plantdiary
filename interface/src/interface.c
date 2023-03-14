/**
 * author: Brando
 * date: 2/16/23
 */

#include "interface.h"
#include <bflibc/bflibc.h>
#include <stdlib.h>
#include <stdio.h>

int InterfaceGetUserDataPath(char * path) {
	int result = 0;

	char * homePath = BFFileSystemPathCopyHomePath(&result);
	if (result == 0) {
		sprintf(path, "%s/%s", homePath, kInterfacePlantdiaryUserDataDirname);
		BFFree(homePath);
		return 0;
	} else {
		return -1;
	}
}

