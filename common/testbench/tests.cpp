/**
 * author: Brando
 * date: 4/14/23
 */

#include "values_tests.hpp"

int main(int argc, char * argv[]) {
	int pass = 0, fail = 0;
	float tp = 0, tf = 0;

	printf("\n---------------------------\n");
	values_tests(&pass, &fail);
	printf("[+ %d, - %d]\n", pass, fail);
	tp += pass; tf += fail;
	pass = 0; fail = 0;
	
	printf("\n");
	printf("Grade - %.2f%% (%d/%d)\n", (tp / (tp + tf)) * 100, (int) tp, (int) (tp + tf));

	return 0;
}

