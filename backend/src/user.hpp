/**
 * author: brando
 * date: 1/5/23
 *
 */

#ifndef USERS_HPP
#define USERS_HPP

#include "accessorspecifiers.hpp"
#include <uuid/uuid.h>
#include "list.hpp"
#include <common.h>

class Plant;

class User {
PUBLIC:

	~User();

	static User * createUser(
		const char * uuid,
		const char * username,
		const char * firstName,
		const char * lastName,
		int * err
	);

	/**
	 * Initially finds all of user's plants
	 */
	int loadPlants();

	int plantCount();

	const char * uuid();
	const char * username();
	const char * firstname();
	const char * lastname();
	const char * sessionID();

PRIVATE:

	User();	

	/**
	 * Plants we own
	 */
	List<Plant *> _plants;

	char _uuid[UUID_STR_LEN];
	char _firstName[kPDCommonUsernameMaxLength];
	char _lastName[kPDCommonUsernameMaxLength];
	char _userName[kPDCommonUsernameMaxLength];
	char _sessionID[UUID_STR_LEN];
};

#endif // USERS_HPP

