/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef DATABASE_TESTS_HPP
#define DATABASE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <database.hpp>
#include <delete.hpp>
#include <string.h>

int test_DatabaseInit() {
	int error = Database::initialize();

	if (error == 0) Database::deinitialize();

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void database_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_DatabaseInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // DATABASE_TESTS_HPP

