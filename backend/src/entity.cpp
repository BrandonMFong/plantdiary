/**
 * author: Brando
 * date: 3/13/23
 */

#include "entity.hpp"
#include <cstring>

void Entity::release(Entity * e) {
	delete e;
}

Entity::Entity(const char * uuid) {
	this->_uuid[0] = '\0';

	if (uuid) {
		strcpy(this->_uuid, uuid);
	}
}

Entity::~Entity() {

}

const char * Entity::uuid() const {
	return this->_uuid;
}

