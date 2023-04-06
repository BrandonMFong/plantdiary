/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef LOGGER_TESTS_HPP
#define LOGGER_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <logger.hpp>
#include <bflibcpp/delete.hpp>

int test_LoggerInit() {
	int error = 0;

	error = Logger::initialize();

	if (error == 0) Logger::deinitialize();

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void logger_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_LoggerInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // LOGGER_TESTS_HPP

