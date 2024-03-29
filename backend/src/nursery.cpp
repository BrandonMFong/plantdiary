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

int Nursery::createNewPlantForUser(const char * userUUID, const char * name, const char * speciesName, const Time * birthDate) {
	int result = 0;
	char uuidStr[kBFStringUUIDStringLength];

	if (result == 0) {
		BFStringGetRandomUUIDString(uuidStr);
		result = Database::shared()->setNewPlant(name, uuidStr, speciesName, birthDate, userUUID);
	}

	return result;
}

int Nursery::modifyPlant(const char * plantUUID, const char * name, const char * species) {
	int result = 0;

	BFDLog("modify");

	result = Database::shared()->modifyPlant(plantUUID, name, species);
	return result;
}

int Nursery::copyPlantListForUserUUID(const char * userUUID, List<Plant *> * plants) {
	return Database::shared()->copyPlantListForUserUUID(userUUID, plants);
}

