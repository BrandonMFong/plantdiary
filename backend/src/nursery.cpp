/**
 * author: Brando
 * date: 4/5/23
 */

#include "nursery.hpp"
#include <stdlib.h>
#include "plant.hpp"
#include "user.hpp"
#include "pool.hpp"
#include "database.hpp"
#include <bflibcpp/bflibcpp.hpp>

using namespace BF;

Nursery * gNursery = NULL;

int Nursery::initialize() {
	if (gNursery = new Nursery) return 0;
	return 1;
}

void Nursery::deinitialize() {
	Delete(gNursery);
}

Nursery * Nursery::shared() {
	return gNursery;
}

Nursery::Nursery() {

}

Nursery::~Nursery() {

}

int Nursery::createNewPlant(const char * userSessionID, const char * name, const char * speciesName, const Time * birthDate) {
	int result = 0;
	char uuidStr[kBFStringUUIDStringLength];

	User * user = Pool::shared()->getUserForSessionID(userSessionID);
	if (user == NULL) {
		result = 56;
		BFDLog("Could not find user for session id: %s", userSessionID);
	}

	if (result == 0) {
		BFStringGetRandomUUIDString(uuidStr);
		result = Database::shared()->setNewPlant(name, uuidStr, speciesName, birthDate, user->uuid());
	}

	return result;
}

