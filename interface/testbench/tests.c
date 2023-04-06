/**
 * author: Brando
 * date: 2/9/23
 */

#include "session_tests.h"
#include "version_tests.h"
#include "arguments_tests.h"
#include <stdio.h>

int main() {
	int pass = 0, fail = 0;
	float tp = 0, tf = 0;

	printf("\n---------------------------\n");
	session_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	version_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("---------------------------\n");
	arguments_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;

	printf("Grade - %.2f%% (%d/%d)\n", (tp / (tp + tf)) * 100, (int) tp, (int) (tp + tf));


	return 0;
}

