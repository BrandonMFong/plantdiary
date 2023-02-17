/**
 * author: Brando
 * date: 2/16/23
 */

#include "interface.h"
#include <clib.h>
#include <stdlib.h>
#include <stdio.h>

int InterfaceGetUserDataPath(char * path) {
	int result = 0;

	char * homePath = CopyHomePath(&result);
	if (result == 0) {
		sprintf(path, "%s/%s", homePath, kInterfacePlantdiaryUserDataDirname);
		Free(homePath);
		return 0;
	} else {
		return -1;
	}
}

