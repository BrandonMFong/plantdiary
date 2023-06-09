/**
 * author: brando
 * date: 1/5/23
 *
 */

#ifndef USERS_HPP
#define USERS_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <bflibcpp/list.hpp>
#include <common.h>
#include "entity.hpp"

extern "C" {
#include <bflibc/stringutils.h>
}

class Plant;

class User : public Entity {
PUBLIC:
	static void release(User * u);
	~User();

	static User * createUser(
		const char * uuid,
		const char * username,
		const char * firstName,
		const char * lastName,
		int * err
	);

	int plantCount();
	const Plant * plantForUUID(const char * plantUUID, int * err);

	const char * username() const;
	const char * firstname() const;
	const char * lastname() const;
	const char * sessionID() const;
	
	Entity::Type type() const;

	/**
	 * Initially finds all of user's plants
	 */
	int loadPlants();

	/**
	 * Clears plant array and calls loadPlants to get a clear slate
	 *
	 * Will this affect performance if we have a lot of plants
	 */
	int reloadPlants();

PRIVATE:

	User(const char * uuid);	

	/**
	 * Plants we own
	 */
	BF::List<Plant *> _plants;

	char _firstName[kPDCommonUsernameMaxLength];
	char _lastName[kPDCommonUsernameMaxLength];
	char _userName[kPDCommonUsernameMaxLength];
	char _sessionID[kBFStringUUIDStringLength];
};

#endif // USERS_HPP

