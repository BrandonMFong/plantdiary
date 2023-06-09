/**
 * author: Brando
 * date: 1/5/22
 */

#include "pool.hpp"
#include "database.hpp"
#include <bflibcpp/delete.hpp>
#include "logger.hpp"
#include "user.hpp"
#include <string.h>

using namespace BF;

Pool * gPool = 0;

Pool::Pool() {
	this->_users.setDeallocateCallback(User::release);
}

Pool::~Pool() {}

int Pool::initialize() {
	int result = 0;
	if ((gPool = new Pool) == NULL) {
		result = 1;
	}

	if (result == 0) {
		PDLog("Pool initialized");
	}

	return result;
}

void Pool::deinitialize() {
	Delete(gPool);
}

Pool * Pool::shared() {
	return gPool;
}

int Pool::activateUser(const char * username, const char * password, char * sessionID) {
	User * user = 0;
	int result = Database::shared()->getUserForCredentials(username, password, &user);
	if (user) {
		if (result == 0) {
			BFDLog("%x", user);
			BFDLog("error %d", result);
			BFDLog("Username: %s", user->username());
			BFDLog("uuid: %s", user->uuid());
			BFDLog("first: %s", user->firstname());
			BFDLog("last: %s", user->lastname());
			BFDLog("session id: %s", user->sessionID());

			if (!this->containsUserWithUUID(user->uuid())) {
				result = this->_users.add(user);
			} else {
				BFDLog("user already is activited");
				User * tmp = user;
				user = this->getUserForUUID(tmp->uuid());
				Delete(tmp);
			}
		}

		if (result == 0) {
			strcpy(sessionID, user->sessionID());
		}
	}

	return 0;
}

bool Pool::containsUserWithUUID(const char * uuid) {
	List<User *>::Node * n = 0;
	for (n = this->_users.first(); n != NULL; n = n->next()) {
		if (!strcmp(n->object()->uuid(), uuid)) return true;
	}
	return false;
}

User * Pool::getUserForUUID(const char * uuid) {
	List<User *>::Node * n = 0;
	for (n = this->_users.first(); n != NULL; n = n->next()) {
		if (!strcmp(n->object()->uuid(), uuid)) return n->object();
	}
	return NULL;
}

User * Pool::getUserForSessionID(const char * sessionID) {
	List<User *>::Node * n = 0;
	for (n = this->_users.first(); n != NULL; n = n->next()) {
		if (!strcmp(n->object()->sessionID(), sessionID)) return n->object();
	}
	return NULL;
}

int Pool::deactivateUserWithSessionID(const char * sessionID) {
	List<User *>::Node * n = 0;
	int i = 0;
	for (n = this->_users.first(); n != NULL; n = n->next()) {
		if (!strcmp(n->object()->sessionID(), sessionID)) {
			return this->_users.deleteObjectAtIndex(i);
		}

		i++;
	}

	return 0;
}

int Pool::activeUserCount() {
	return this->_users.count();
}

