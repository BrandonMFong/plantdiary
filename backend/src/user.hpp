/**
 * author: brando
 * date: 1/5/23
 *
 */

#ifndef USERS_HPP
#define USERS_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include <uuid/uuid.h>
#include <bflibcpp/list.hpp>
#include <common.h>
#include "entity.hpp"

class Plant;

class User : public Entity {
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

	const char * username();
	const char * firstname();
	const char * lastname();
	const char * sessionID();

	Entity::Type type();

PRIVATE:

	User(const char * uuid);	

	/**
	 * Plants we own
	 */
	BF::List<Plant *> _plants;

	char _firstName[kPDCommonUsernameMaxLength];
	char _lastName[kPDCommonUsernameMaxLength];
	char _userName[kPDCommonUsernameMaxLength];
	char _sessionID[UUID_STR_LEN];
};

#endif // USERS_HPP

