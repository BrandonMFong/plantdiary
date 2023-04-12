/**
 * author: Brando
 * date: 11/29/22
 *
 */

#ifndef PLANT_HPP
#define PLANT_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <uuid/uuid.h>
#include <internal/limits.h>
#include "entity.hpp"

class Nursery;

class Plant : public Entity {
	friend class Nursery;
PUBLIC:
	~Plant();

	/// Returns the entity type Plant
	Entity::Type type() const;

	/**
	 * Returns the uuid type for species
	 */
	const char * species() const;

PRIVATE:
	Plant();

	char _name[kPDCommonPlantNameStringLength];

	/// Holds species type
	char _species[kPDCommonPlantSpeciesStringLength];
};

#endif // PLANT_HPP

