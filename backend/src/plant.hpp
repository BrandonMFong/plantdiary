/**
 * author: Brando
 * date: 11/29/22
 *
 */

#ifndef PLANT_HPP
#define PLANT_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <bflibcpp/time.hpp>
#include <uuid/uuid.h>
#include <internal/limits.h>
#include "entity.hpp"

extern "C" {
#include <bflibc/stringutils.h>
}

class Nursery;

class Plant : public Entity {
	friend class Nursery;
PUBLIC:
	static void release(Plant * p);
	~Plant();
	static Plant * createPlant(const char * name, const char * species, BFTime birthTime, BFTime deathTime, BFTime ownershipTime, int * err);

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

