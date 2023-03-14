/**
 * author: Brando
 * date: 3/13/23
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <uuid/uuid.h>
#include <bflibcpp/accessorspecifiers.hpp>

class Entity {
PUBLIC:
	Entity(const char * uuid);
	virtual ~Entity();

	const char * uuid();

PRIVATE:

	char _uuid[UUID_STR_LEN];
};

#endif // ENTITY_HPP

