/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef USER_TESTS_HPP
#define USER_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <user.hpp>
#include <delete.hpp>

int test_userinit() {
	int error = 0;

	User * u = User::createUser("uuid", "brandonmfong", "brandon", "fong", &error);
	PRINT_TEST_RESULTS(error == 0);
	Delete(u);

	return error;
}

void user_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_userinit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // USER_TESTS_HPP

