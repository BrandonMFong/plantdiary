/**
 * author: Brando
 * date: 11/29/22
 *
 */

#include "plant.hpp"
#include <string.h>

void Plant::release(Plant * p) {
	delete p;
}

Plant::Plant(const char * uuid) : Entity(uuid) {
	this->_name[0] = '\0';
	this->_species[0] = '\0';
}

Plant::~Plant() {

}

Entity::Type Plant::type() const {
	return kEntityTypePlant;
}

const char * Plant::name() const {
	return this->_name;
}

Plant * Plant::createPlant(
	const char * uuid,
	const char * name,
	const char * species,
	BFTime birthTime,
	BFTime deathTime, 
	BFTime ownershipTime,
	int * err
) {
	int error = 0;
	Plant * result = new Plant(uuid);

	if (!result) {
		error = 3;
	} else {
		strcpy(result->_name, name);
		strcpy(result->_species, species);
		result->_birth = birthTime;
		result->_death = deathTime;
		result->_ownership = ownershipTime;
	}

	if (err) { *err = error; }

	return result;
}
