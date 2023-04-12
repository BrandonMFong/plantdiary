/**
 * author: Brando
 * date: 3/8/23
 */

#include "instructorset.hpp"
#include <bflibcpp/bflibcpp.hpp>
#include <common.h>
#include <external/json-parser/json.h>
#include "database.hpp"
#include "pipe.hpp"
#include "pool.hpp"
#include "nursery.hpp"
#include <uuid/uuid.h>

using namespace BF;

InstructorSet::InstructorSet(PDInstruction * instr) : Instructor(instr) {

}

InstructorSet::~InstructorSet() {

}

int InstructorSet::execute() {
	switch (this->subCommand()) {
	case kPDSubCommandSetEvent:
		return this->executeEvent();
	case kPDSubCommandSetPlant:
		return this->executePlant();
	default:
		BFDLog("Unknown sub command: %d", this->subCommand());
		return 53;
	}
}

int InstructorSet::executePlant() {
	int result = 0;
	char data[kPDInstructionDataMaxLength];
	short length = 0;
	char sessionID[kBFStringUUIDStringLength];
	char plantUUID[kBFStringUUIDStringLength];
	char plantName[kPDCommonPlantNameStringLength];
	char plantSpecies[kPDCommonPlantSpeciesStringLength];
	PDSetPlantOption option = kPDSetPlantOptionNone;
	Time * tm = NULL;

	BFDLog("Set for plant");
	this->getData(data, &length);
	BFDLog("Data received: %s", data);

	// Parse the json data	
	json_value * val = json_parse(data, length);
	int l = val->u.object.length;
	if (val == NULL) {
		result = 66;
	} else if (l != 5) {
		BFDLog("There are %d objects, unexpected", l);
		result = 65;
	} else {
		for (int i = 0; i < l; i++) {
			if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantName)) {
				strcpy(plantName, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySessionID)) {
				strcpy(sessionID, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantSpecies)) {
				strcpy(plantSpecies, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantUUID)) {
				strcpy(plantUUID, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantOption)) {
				option = (PDSetPlantOption) val->u.object.values[i].value->u.integer;
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantBirthdate)) {
				tm = new Time(val->u.object.values[i].value->u.integer);
			}
		}

		BFDLog("Session ID: %s", sessionID);
		BFDLog("Plant uuid: %s", plantUUID);
		BFDLog("option: %x", option);
		BFDLog("plant name: %s", plantName);
		BFDLog("plant species: %s", plantSpecies);
	}

	// Create new plant under the user
	if (result == 0) {
		switch (option) {
		case kPDSetPlantOptionNew:
			result = Nursery::shared()->createNewPlant(sessionID, plantName, plantSpecies, tm);
			break;
		case kPDSetPlantOptionModify:
			result = Nursery::shared()->modifyNewPlant(plantUUID, plantName, plantSpecies);
			break;
		case kPDSetPlantOptionNone:
			break;
		default:
			BFDLog("Unknown option: %d", option);
			result = 68;
			break;
		}
	}

	// Send information about the new plant
	if (result == 0) {
		BFDLog("Sending response");
		PDResponse response;
		strcpy(response.data, "{}");
		response.length = strlen(strcpy(response.data, "{}"));
		result = Pipe::shared()->writeResponse(&response);
		BFDLog("Sent response");
	}

	if (result) {
		BFDLog("Error: %d", result);
	}

	return result;
}

int InstructorSet::executeEvent() {
	char eventType[kPDCommonEventTypeStringLength];
	char sessionID[kBFStringUUIDStringLength];
	char data[kPDInstructionDataMaxLength];
	short length = 0;
	Time * tm = NULL;
	List<Entity *> participants;
	User * user = NULL;

	BFDLog("An event will be logged");

	this->getData(data, &length);

	BFDLog("Data received: %s", data);

	json_value * val = json_parse(data, length);
	int l = val->u.object.length;
	int result = 0;
	if (val == NULL) {
		result = 54;
	} else if (l != 3) {
		result = 55;
	} else {
		for (int i = 0; i < l; i++) {
			if (!strcmp(val->u.object.values[i].name, kPDKeySetEventType)) {
				strcpy(eventType, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySessionID)) {
				strcpy(sessionID, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySetEventCurrentTime)) {
				tm = new Time(val->u.object.values[i].value->u.integer);
			}
		}

		BFDLog("Session ID: %s", sessionID);
		BFDLog("event type: %s", eventType);
		BFDLog("epoch value: %ld", tm->epoch());
	}

	if (result == 0) {
		user = Pool::shared()->getUserForSessionID(sessionID);
		if (user == NULL) {
			result =  56;
			BFDLog("Could not find user for session id: %s", sessionID);
		}
	}

	char eventUUID[kBFStringUUIDStringLength];
	if (result == 0) {
		//participants.add((Entity *) user);
		BFStringGetRandomUUIDString(eventUUID);
		result = Database::shared()->setEvent(eventType, tm, eventUUID, (const Entity *) user, &participants);
	}

	// Send information about the event
	if (result == 0) {
		PDResponse response;
		strcpy(response.data, "{}");
		response.length = strlen(strcpy(response.data, "{}"));
		result = Pipe::shared()->writeResponse(&response);
	}

	Delete(tm);

	return 0;
}

