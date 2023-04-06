/**
 * author: Brando
 * date: 4/5/23
 */

#include "nursery.hpp"
#include <stdlib.h>
#include "plant.hpp"
#include "user.hpp"
#include "pool.hpp"
#include <bflibcpp/bflibcpp.hpp>

Nursery * gNursery = NULL;

int Nursery::initialize() {
	return 0;
}

void Nursery::deinitialize() {

}

Nursery * Nursery::shared() {
	return NULL;
}

Nursery::Nursery() {

}

Nursery::~Nursery() {

}

int Nursery::createNewPlant(const char * userSessionID, const char * name) {
	int result = 0;

	User * user = Pool::shared()->getUserForSessionID(userSessionID);
	if (user == NULL) {
		result = 56;
		BFDLog("Could not find user for session id: %s", userSessionID);
	}

	return result;
}

