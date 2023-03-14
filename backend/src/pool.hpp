/**
 * author: Brando
 * date: 1/5/22
 *
 * User pool
 */

#ifndef POOL_HPP
#define POOL_HPP

#include <bflibcpp/list.hpp>
#include <bflibcpp/accessorspecifiers.hpp>

class User;

class Pool {
PUBLIC:
	static int initialize();
	static void deinitialize();

	/**
	 * Shared instance that is responsible for the pool of users
	 */
	static Pool * shared();

	/**
	 * Activates user with credentials and returns sessionID
	 *
	 * sessionID: Uniquely identifies the user for current sesion
	 */
	int activateUser(const char * username, const char * password, char * sessionID);

	/**
	 * returns null if no user was found
	 */
	User * getUserForUUID(const char * uuid);

	/**
	 * returns null if no user was found
	 */
	User * getUserForSessionID(const char * sessionID);

	/**
	 * Deletes user from pool
	 */
	int deactivateUserWithSessionID(const char * sessionID);

	/**
	 * Returns the number of users in the users array
	 */
	int activeUserCount();

PRIVATE:
	Pool();
	~Pool();

	/**
	 * Checks to see if it already has a user with the uuid
	 */
	bool containsUserWithUUID(const char * uuid);

	BF::List<User *> _users;
};

#endif // POOL_HPP

