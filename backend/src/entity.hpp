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

	typedef enum {
		kEntityTypeUser = 0,
		kEntityTypePlant = 1
	} Type;

	Entity(const char * uuid);
	virtual ~Entity();

	const char * uuid() const;

	virtual Type type() const = 0;

PRIVATE:

	char _uuid[UUID_STR_LEN];
};

#endif // ENTITY_HPP

