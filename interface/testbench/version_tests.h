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

void version_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // VERSION_TESTS_H

