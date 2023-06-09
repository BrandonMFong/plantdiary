/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef IV_TESTS_HPP
#define IV_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <instructorversion.hpp>
#include <bflibcpp/delete.hpp>
#include <string.h>

int test_IVInit() {
	int error = 0;

	PDInstruction i;
	strcpy(i.data, "{\"key\" : \"value\"}");
	i.length = strlen(i.data);
	InstructorVersion * iv = new InstructorVersion(&i);

	if (iv == NULL) error = 1;

	Delete(iv);

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void iv_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_IVInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // IV_TESTS_HPP

