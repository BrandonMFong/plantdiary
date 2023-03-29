/**
 * author: Brando
 * date: 3/8/23
 */

#include "instructorset.hpp"
#include <bflibcpp/bflibcpp.hpp>
#include <common.h>
#include <external/json-parser/json.h>
#include "database.hpp"
#include "pool.hpp"
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
	default:
		BFDLog("Unknown sub command: %d", this->subCommand());
		return 53;
	}
}

int InstructorSet::executeEvent() {
	char eventType[kPDCommonEventTypeStringLength];
	char sessionID[UUID_STR_LEN];
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
		}
	}

	if (result == 0) {
		//participants.add(user); // we need to make sure list does not release memory
		result = Database::shared()->saveEvent(eventType, tm, &participants);
	}

	Delete(tm);

	return 0;
}

