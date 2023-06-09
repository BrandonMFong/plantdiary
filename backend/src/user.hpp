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
	Plant * plantForUUID(const char * plantUUID, int * err);

	const char * username();
	const char * firstname();
	const char * lastname();
	const char * sessionID();

	Entity::Type type() const;

PRIVATE:

	User(const char * uuid);	

	/**
	 * Initially finds all of user's plants
	 */
	int loadPlants();

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

