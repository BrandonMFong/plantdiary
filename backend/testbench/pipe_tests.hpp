/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef PIPE_TESTS_HPP
#define PIPE_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <pipe.hpp>
#include <bflibcpp/delete.hpp>

int test_PipeInit() {
	int error = 0;

	error = Pipe::initialize();

	if (error == 0) Pipe::deinitialize();

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void pipe_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_PipeInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // PIPE_TESTS_HPP

