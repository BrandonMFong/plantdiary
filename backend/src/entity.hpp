/**
 * author: Brando
 * date: 3/13/23
 */

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <uuid/uuid.h>
#include <bflibcpp/accessorspecifiers.hpp>

/**
 * Describes objects in pd
 *
 * Every entity do not need to be instantiated in the app.  I.e., if we can 
 * do things to an object with only involving the database then we should
 * only be doing that
 */
class Entity {
PUBLIC:
	static void release(Entity * e);
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

