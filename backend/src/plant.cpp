/**
 * author: Brando
 * date: 11/29/22
 *
 */

#include "plant.hpp"

void Plant::release(Plant * p) {
	delete p;
}

Plant::Plant() : Entity(0) {
	this->_name[0] = '\0';
}

Plant::~Plant() {

}

const char * Plant::uuid() const {
	return this->_uuid;
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
