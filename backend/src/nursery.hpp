/**
 * author: Brando
 * date: 4/5/23
 */

#ifndef NURSERY_HPP
#define NURSERY_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <bflibcpp/time.hpp>
#include <bflibcpp/list.hpp>

class Plant;

class Nursery {
PUBLIC:

	static int initialize();
	static void deinitialize();
	static Nursery * shared();

	/**
	 * Inserts a new plant into the database
	 *
	 * userSessionID: The id for the user that is creating plant
	 */
	int createNewPlant(const char * userSessionID, const char * name, const char * speciesName, const BF::Time * birthDate);

	/**
	 * modifies plant with uuid
	 */
	int modifyPlant(const char * plantUUID, const char * name, const char * species);

	//Plant * plantForUUID(const char * plantUUID, int * err);
	int copyPlantListForUserUUID(const char * userUUID, BF::List<Plant *> * plants);

PRIVATE:
	Nursery();
	~Nursery();
};

#endif // NURSERY_HPP

