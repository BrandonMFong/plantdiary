/**
 * author: Brando
 * date: 11/29/22
 *
 * This is for Camille.
 */

#include "driver.hpp"

extern "C" {
#include <bflibc/coreutils.h>
}

int main(void) {
	int error = Driver::initialize();

	if (error) {
		BFErrorPrint("%d", error);
	} else {
		return Driver::run();
	}
}

