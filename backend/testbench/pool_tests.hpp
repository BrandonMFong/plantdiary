/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef POOL_TESTS_HPP
#define POOL_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <pool.hpp>
#include <bflibcpp/delete.hpp>

int test_PoolInit() {
	int error = 0;

	Pool * p = new Pool;
	if (p == NULL) error = 1;

	if (error == 0) error = Pool::initialize();

	if (error == 0) Pool::deinitialize();
	
	Delete(p);

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void pool_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_PoolInit() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // POOL_TESTS_HPP

