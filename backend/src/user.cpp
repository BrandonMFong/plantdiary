/**
 * author: brando
 * date: 1/5/23
 *
 */

#include "user.hpp"
#include <string.h>

User * User::createUser(
	const char * uuid, 
	const char * username, 
	const char * firstName, 
	const char * lastName, 
	int * err
) {
	if (!uuid || !firstName) {
		if (err) *err = 1;
		return NULL;
	} else {
		User * res = new User(uuid);
		strcpy(res->_userName, username);
		strcpy(res->_firstName, firstName);

		if (lastName) strcpy(res->_lastName, lastName);

		// Create uuid for user
		uuid_t bin;
		uuid_generate_random(bin);
		uuid_unparse_lower(bin, res->_sessionID);

		return res;
	}
}

User::User(const char * uuid) : Entity(uuid) {
	this->_firstName[0] = '\0';
	this->_lastName[0] = '\0';
	this->_userName[0] = '\0';
	this->_sessionID[0] = '\0';
}

User::~User() {

}

int User::loadPlants() {
	return 0;
}

int User::plantCount() {
	return this->_plants.count();
}

const char * User::username() {
	return this->_userName;
}

const char * User::firstname() {
	return this->_firstName;
}

const char * User::lastname() {
	return this->_lastName;
}

const char * User::sessionID() {
	return this->_sessionID;
}

