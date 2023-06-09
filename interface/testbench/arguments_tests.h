/**
 * author: Brando
 * date: 4/6/23
 */

#ifndef ARGUMENTS_TESTS_H
#define ARGUMENTS_TESTS_H

#include "clib_tests.h"
#include <stdio.h>
#include <typearguments.h>
#include <arguments.h>
#include <stdlib.h>
#include <string.h>

int test_NewPlants(void) {
	int error = 0;
	Arguments args;
	char * myname = "myname";
	char * argv[] = {"tool", "set", "plant", "-name", myname, "--new"};
	ArgumentsParseArguments(sizeof(argv)/sizeof(argv[0]), argv, &args);

	if (!args.type.set.plant.newPlant) {
		error = 1;
	} else if (strcmp(args.type.set.plant.name, myname)) {
		error = 2;
	}

	PRINT_TEST_RESULTS(error == 0);

	return error;
}

void arguments_tests(int * pass, int * fail) {
	int p = 0, f = 0;

	INTRO_TEST_FUNCTION;

	if (!test_NewPlants()) p++;
	else f++;

	if (pass) *pass += p;
	if (fail) *fail += f;
}

#endif // ARGUMENTS_TESTS_H

