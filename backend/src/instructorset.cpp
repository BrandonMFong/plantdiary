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
	char plantName[kPDCommonPlantNameStringLength];
	bool isNew = false;

	BFDLog("Set for plant");
	
	this->getData(data, &length);

	BFDLog("Data received: %s", data);

	// Parse the json data	
	json_value * val = json_parse(data, length);
	int l = val->u.object.length;
	if (val == NULL) {
		result = 66;
	} else if (l != 3) {
		result = 65;
	} else {
		for (int i = 0; i < l; i++) {
			if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantName)) {
				strcpy(plantName, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySessionID)) {
				strcpy(sessionID, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeySetPlantIsNew)) {
				isNew = val->u.object.values[i].value->u.boolean;
			}
		}

		BFDLog("Session ID: %s", sessionID);
		BFDLog("is new: %d", isNew);
		BFDLog("plant name: %s", plantName);
	}

	// Create new plant under the user
	if (result == 0) {
		result = Nursery::shared()->createNewPlant(plantName);
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

	if (result == 0) {
		//participants.add((Entity *) user);
		result = Database::shared()->saveEvent(eventType, tm, (const Entity *) user, &participants);
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

