/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef DRIVER_TESTS_HPP
#define DRIVER_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <driver.hpp>
#include <bflibcpp/delete.hpp>
#include <string.h>

int test_DriverInit() {
	int error = Driver::initialize();

	if (error == 0) Driver::deinitialize();

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void driver_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_DriverInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // DRIVER_TESTS_HPP

