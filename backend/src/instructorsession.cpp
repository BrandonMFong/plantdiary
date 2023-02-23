/**
 * author: Brando
 * date: 2/3/23
 */

#include "instructorsession.hpp"
#include "pool.hpp"
#include "pipe.hpp"
#include "user.hpp"
#include <log.h>
#include <common.h>
#include <stdio.h>
#include <external/json-parser/json.h>
#include <uuid/uuid.h>

InstructorSession::InstructorSession(PDInstruction * instr) : Instructor(instr) {

}

InstructorSession::~InstructorSession() {

}

int InstructorSession::execute() {
	switch (this->subCommand()) {
	case kPDSubCommandSessionStart:
		return this->executeStart();
	case kPDSubCommandSessionStatus:
		return this->executeStatus();
	case kPDSubCommandSessionStop:
		return this->executeStop();
	default:
		return -19;
	}
}

int InstructorSession::executeStart() {
	int result = 0;
	char data[kPDInstructionDataMaxLength];
	short length = 0;
	char username[kPDCommonUsernameMaxLength];
	char passwd[kPDCommonPasswordMaxLength];
	char sessionID[UUID_STR_LEN];
	PDResponse resp;

	this->getData(data, &length);
	DLog("command: session");
	DLog("json: %s", data);

	json_value * val = json_parse(data, length);

	int l = val->u.object.length;
	if (val == NULL) {
		result = 1;
	} else if (l != 2) {
		result = 2;
	} else {
		for (int i = 0; i < l; i++) {
			if (!strcmp(val->u.object.values[i].name, kPDKeyUsername)) {
				strcpy(username, val->u.object.values[i].value->u.string.ptr);
			} else if (!strcmp(val->u.object.values[i].name, kPDKeyPassword)) {
				strcpy(passwd, val->u.object.values[i].value->u.string.ptr);
			}
		}

		DLog("username: %s\n", username);
		DLog("password: %s\n", passwd);

		result = Pool::shared()->activateUser(username, passwd, sessionID);
	}

	if (result == 0) {
		resp.length = snprintf(resp.data, kPDResponseDataMaxLength, kPDJsonSessionID, kPDKeySessionID, sessionID);
		Pipe::shared()->writeResponse(&resp);
	}

	json_value_free(val);
	
	return result;
}

int InstructorSession::executeStatus() {
	int result = 0;
	char data[kPDInstructionDataMaxLength];
	short length = 0;
	char sessionID[UUID_STR_LEN];
	PDResponse resp;
	User * user = NULL;
	
	DLog("command: session status");

	this->getData(data, &length);

	DLog("Parsing: %s", data);
	json_value * val = json_parse(data, length);
	if (val->u.object.length != 1) {
		result = 17;
		DLog("error with json data, length is %d", val->u.object.length);
	} else if (strcmp(val->u.object.values[0].name, kPDKeySessionID)) {
		DLog("Incorrect key for session start: %s", val->u.object.values[0].name);
		result = 19; 
	} else {
		strcpy(sessionID, val->u.object.values[0].value->u.string.ptr);
		DLog("Session id: %s", sessionID);

		user = Pool::shared()->getUserForSessionID(sessionID);

		if (user) {
			resp.length = snprintf(
				resp.data,
				kPDResponseDataMaxLength,
				kPDJsonSessionStatus,
				kPDKeyUsername,
				user->username(),
				kPDKeySessionID,
				user->sessionID()
			);

			Pipe::shared()->writeResponse(&resp);
		}
	}

	json_value_free(val);

	return result;
}

int InstructorSession::executeStop() {
	int result = 0;
	char data[kPDInstructionDataMaxLength];
	short length = 0;
	char sessionID[UUID_STR_LEN];
	PDResponse resp;
	
	DLog("command: session stop");

	this->getData(data, &length);

	json_value * val = json_parse(data, length);
	if (val->u.object.length != 1) {
		result = 31;
		DLog("error with json data, length is %d", val->u.object.length);
	} else if (strcmp(val->u.object.values[0].name, kPDKeySessionID)) {
		DLog("Incorrect key for session start: %s", val->u.object.values[0].name);
		result = 32; 
	} else {
		strcpy(sessionID, val->u.object.values[0].value->u.string.ptr);

		result = Pool::shared()->deactivateUserWithSessionID(sessionID);
	}

#ifdef DEBUG
	if (result == 0) {
		DLog("Deactivated session: %s", sessionID);
		DLog("Remaining user count: %d", Pool::shared()->activeUserCount());
	}
#endif

	DLog("error code to send out: %d", result);
	// Will always send something
	resp.length = snprintf(
		resp.data,
		kPDResponseDataMaxLength,
		kPDJsonSessionStop,
		kPDKeySessionStop,
		result
	);

	if (Pipe::shared()->writeResponse(&resp) != 0) {
		DLog("Error sending response");
	}

	json_value_free(val);

	return result;

}

