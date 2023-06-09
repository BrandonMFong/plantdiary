/**
 * author: brando
 * date: 1/5/23
 *
 */

#include "user.hpp"
#include <bflibcpp/bflibcpp.hpp>
#include <string.h>
#include "plant.hpp"
#include "nursery.hpp"

using namespace BF;

void User::release(User * u) {
	delete u;
}

User * User::createUser(
	const char * uuid, 
	const char * username, 
	const char * firstName, 
	const char * lastName, 
	int * err
) {
	int error = 0;
	if (!uuid || !firstName) {
		if (err) *err = 1;
		return NULL;
	} else {
		User * res = new User(uuid);
		strcpy(res->_userName, username);
		strcpy(res->_firstName, firstName);

		if (lastName) strcpy(res->_lastName, lastName);

		// Create uuid for user session
		BFStringGetRandomUUIDString(res->_sessionID);

		error = res->loadPlants();

		if (err) *err = error;

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
	int result = 0;
	return Nursery::shared()->copyPlantListForUserUUID(this->uuid(), &this->_plants);
}

const Plant * User::plantForUUID(const char * plantUUID, int * err) {
	List<Plant *>::Node * n = this->_plants.first();
	for(; n; n = n->next()) {
		if (!strcmp(n->object()->uuid(), plantUUID)) {
			return n->object();
		}
	}

	if (err) *err = 2;
	return NULL;
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

Entity::Type User::type() const {
	return Entity::Type::kEntityTypeUser;
}

