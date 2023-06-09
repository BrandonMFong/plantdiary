/**
 * author: Brando
 * date: 11/29/22
 *
 */

#include "plant.hpp"

Plant::Plant() : Entity(0) {
	this->_name[0] = '\0';
}

Plant::~Plant() {

}

Entity::Type Plant::type() const {
	return kEntityTypePlant;
}

Plant * Plant::createPlant(const char * name, const char * species, BFTime birthTime, BFTime deathTime, BFTime ownershipTime, int * err) {
	Plant * result = NULL;
	int error = 0;

	if (err) { *err = error; }

	return result;
}
