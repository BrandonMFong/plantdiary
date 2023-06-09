/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef IG_TESTS_HPP
#define IG_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <instructorget.hpp>
#include <bflibcpp/delete.hpp>
#include <string.h>

int test_IGInit() {
	int error = 0;

	PDInstruction i;
	strcpy(i.data, "{\"key\" : \"value\"}");
	i.length = strlen(i.data);
	InstructorGet * ig = new InstructorGet(&i);

	if (ig == NULL) error = 1;

	Delete(ig);

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void ig_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_IGInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // IG_TESTS_HPP

