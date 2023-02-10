/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef VERSION_TESTS_H
#define VERSION_TESTS_H

#include "clib_tests.h"
#include <stdio.h>
#include <version.h>
#include <stdlib.h>

int test_VersionUnknownArg() {
	char * argv[] = {"exec", "session", "unknown"};
	int result = VersionExec(sizeof(argv)/sizeof(argv[0]), argv);
	PRINT_TEST_RESULTS(result == 0);
	return result;
}

void version_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_VersionUnknownArg()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // VERSION_TESTS_H

