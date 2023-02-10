/**
 * author: Brando
 * date: 6/30/22
 */

#include <stdio.h>
#include "user_tests.hpp"
#include "pool_tests.hpp"
#include "plant_tests.hpp"
#include "pipe_tests.hpp"
#include "logger_tests.hpp"
#include "instructorversion_tests.hpp"
#include "instructorsession_tests.hpp"
#include "instructorget_tests.hpp"
#include "driver_tests.hpp"
#include "database_tests.hpp"

int main() {
	int pass = 0, fail = 0;
	float tp = 0, tf = 0;

	printf("\n---------------------------\n");
	user_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	pool_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	plant_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	pipe_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	logger_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	iv_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	is_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	ig_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	driver_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	database_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	printf("\n");
	printf("Grade - %.2f%% (%d/%d)\n", (tp / (tp + tf)) * 100, (int) tp, (int) (tp + tf));

	return 0;
}

