/**
 * author: Brando
 * date: 4/5/23
 */

#include "nursery.hpp"
#include <stdlib.h>
#include "plant.hpp"

Nursery * gNursery = NULL;

int Nursery::initialize() {
	return 0;
}

void Nursery::deinitialize() {

}

Nursery * Nursery::shared() {
	return NULL;
}

Nursery::Nursery() {

}

Nursery::~Nursery() {

}

Plant * Nursery::createNewPlant() {
	Plant * result = NULL;

	return result;
}

