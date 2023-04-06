/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef IS_TESTS_HPP
#define IS_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <instructorsession.hpp>
#include <bflibcpp/delete.hpp>
#include <string.h>

int test_ISInit() {
	int error = 0;

	PDInstruction i;
	strcpy(i.data, "{\"key\" : \"value\"}");
	i.length = strlen(i.data);
	InstructorSession * is = new InstructorSession(&i);

	if (is == NULL) error = 1;

	Delete(is);

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void is_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_ISInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // IS_TESTS_HPP

