/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef SESSION_TESTS_H
#define SESSION_TESTS_H

#include "clib_tests.h"
#include <stdio.h>
#include <session.h>
#include <stdlib.h>

int test_SessionUnknownArg() {
	char * argv[] = {"exec", "session", "unknown"};
	int result = SessionExec(sizeof(argv)/sizeof(argv[0]), argv);
	PRINT_TEST_RESULTS(result != 0);
	return result != 0 ? 0 : result;
}

void session_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_SessionUnknownArg()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // SESSION_TESTS_H

