/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef PLANT_TESTS_HPP
#define PLANT_TESTS_HPP

#define ASSERT_PUBLIC_MEMBER_ACCESS

#include "cpplib_tests.hpp"
#include <plant.hpp>
#include <bflibcpp/delete.hpp>

int test_PlantInit() {
	int error = 0;

	Plant * p = new Plant;
	if (p == NULL) error = 1;

	Delete(p);

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

int test_PlantType() {
	int error = 0;

	Plant * p = new Plant;
	if (p == NULL) error = 1;
	else if(p->type() != Entity::Type::kEntityTypePlant) error = 2;

	Delete(p);

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void plant_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (test_PlantInit() == 0) p++;
	else f++;
	
	if (test_PlantType() == 0) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // PLANT_TESTS_HPP

