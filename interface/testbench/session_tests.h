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

void session_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // SESSION_TESTS_H

