/**
 * author: Brando
 * date: 4/5/23
 */

#ifndef NURSERY_HPP
#define NURSERY_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <bflibcpp/time.hpp>

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
	int createNewPlant(const char * userSessionID, const char * name, const BF::Time * birthDate);

PRIVATE:
	Nursery();
	~Nursery();
};

#endif // NURSERY_HPP

