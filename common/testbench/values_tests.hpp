/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef VALUES_TESTS_HPP
#define VALUES_TESTS_HPP

#include <cpplib_tests.hpp>
#include <stdio.h>
#include <stdlib.h>

void values_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // VALUES_TESTS_HPP

