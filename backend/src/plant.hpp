/**
 * author: Brando
 * date: 11/29/22
 *
 */

#ifndef PLANT_HPP
#define PLANT_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <uuid/uuid.h>
#include "entity.hpp"

#define kPlantNameLength 2 << 7

class Nursery;

class Plant : public Entity {
	friend class Nursery;
PUBLIC:
	~Plant();

	/// Returns the entity type Plant
	Entity::Type type() const;

PRIVATE:
	Plant();

	char _name[kPlantNameLength];
};

#endif // PLANT_HPP

