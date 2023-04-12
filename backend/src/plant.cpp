/**
 * author: Brando
 * date: 11/29/22
 *
 */

#include "plant.hpp"
#include <internal/species.h>
#include <string.h>

Plant::Plant() : Entity(0) {
	this->_name[0] = '\0';

	char tmp[512];
	strcpy(tmp, kPDSpeciesUnknown);
}

Plant::~Plant() {

}

Entity::Type Plant::type() const {
	return kEntityTypePlant;
}

