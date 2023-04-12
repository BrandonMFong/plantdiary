/**
 * author: Brando
 * date: 2/27/23
 */

#include "set.h"
#include <bflibc/bflibc.h>
#include <stdio.h>
#include <string.h>
#include "arguments.h"
#include <common.h>
#include "fifo.h"
#include "session.h"
#include <time.h>
#include <uuid/uuid.h>

int SetEvent(Arguments * args, PDInstruction * i);
int SetPlant(Arguments * args, PDInstruction * instr);

void SetHelp() {
	printf("Argument : %s <sub commands>\n", kArgumentSet);
	printf("  sub commands\n");
	printf("    - '%s' : creates an event for user\n", kArgumentSetEvent);
	printf("      - '%s' <string> : specifies the type of event (%s, %s)\n", kArgumentSetEventType, kArgumentSetEventTypeWater, kArgumentSetEventTypeRepot);
	printf("    - '%s' : creates/modifies plant\n", kArgumentSetPlant);
	printf("      - '%s' <string> : Plant name\n", kArgumentSetPlantName);
	printf("      - '%s' : Creates new plant\n", kArgumentSetPlantNew);

}

int SetExec(Arguments * args) {
	BFDLog("Command: Set");
	PDInstruction i = {0};
	PDInstructionSetCommand(&i, kPDCommandSet);
	
	if (args->subCommand == kPDSubCommandSetEvent) {
		BFDLog("Sub Command: event");
		return SetEvent(args, &i);
	} else if (args->subCommand == kPDSubCommandSetPlant) {
		BFDLog("Sub Command: plant");
		return SetPlant(args, &i);
	} else {
		BFDLog("Unknown sub command %d", args->subCommand);
	}

	return 0;
}

int SetEvent(Arguments * args, PDInstruction * instr) {
	int result = 0;
	char * eventTypes[] = {kPDSetEventTypePlantWater, kPDSetEventTypePlantRepot, kPDSetEventTypeUserLogin};
	PDResponse resp = {0};
	char sessionID[UUID_STR_LEN];
	
	PDInstructionSetSubCommand(instr, kPDSubCommandSetEvent);
	
	if (strlen(args->sessionID) == 0) {
		// If we couldn't find the session id in the arguments then we
		// will look at the cache
		result = SessionGetSessionID(sessionID);
		if (result) {
			BFErrorPrint("Please provide session ID");
			result = 52;
		}
	} else {
		strcpy(sessionID, args->sessionID);
	}

	if (result == 0) {
		if (!BFArrayStringContainsString(eventTypes, sizeof(eventTypes) / sizeof(eventTypes[0]), args->type.set.event.type)) {
			BFDLog("Unknown event: %s", args->type.set.event.type);
			result = 51;
		}
	}

	if (result == 0) {
		instr->length = snprintf(
			instr->data,
			kPDInstructionDataMaxLength,
			kPDJsonSetEvent,
			kPDKeySessionID,
			sessionID,
			kPDKeySetEventType,
			args->type.set.event.type,
			kPDKeySetEventCurrentTime,
			BFTimeGetCurrentTime()
		);

		BFDLog("data: %s", instr->data);
		result = FifoWrite(instr);
	}

	if (result == 0) {
		result = FifoRead(&resp);
		BFDLog("%s", resp.data);
	}

	return result;
}

int SetNewPlant(Arguments * args, PDInstruction * instr, char * sessionID) {
	int result = 0;
	PDResponse resp = {0};
	
	PDInstructionSetSubCommand(instr, kPDSubCommandSetPlant);

	if (!strlen(args->type.set.plant.name)) {
		result = 61;
		BFErrorPrint("Please provide a name for plant");
	}

	// Send data
	if (result == 0) {
		instr->length = snprintf(
			instr->data,
			kPDInstructionDataMaxLength,
			kPDJsonSetNewPlant,
			kPDKeySessionID,
			sessionID,
			kPDKeySetPlantName,
			args->type.set.plant.name,
			kPDKeySetPlantIsNew,
			kPDKeySetPlantBirthdate,
			BFTimeGetCurrentTime()
		);

		BFDLog("data: %s", instr->data);
		result = FifoWrite(instr);
	}

	if (result == 0) {
		result = FifoRead(&resp);
		BFDLog("%s", resp.data);
	}

	return result;

}

int SetPlant(Arguments * args, PDInstruction * instr) {
	int result = 0;
	char sessionID[UUID_STR_LEN];
	
	PDInstructionSetSubCommand(instr, kPDSubCommandSetPlant);

	if (args == 0) {
		result = 62;
	}

	if (result == 0) {	
		if (strlen(args->sessionID) == 0) {
			// If we couldn't find the session id in the arguments then we
			// will look at the cache
			result = SessionGetSessionID(sessionID);
			if (result) {
				BFErrorPrint("Please provide session ID");
				result = 60;
			}
		} else {
			strcpy(sessionID, args->sessionID);
		}
	}

	if (result == 0) {
		if (args->type.set.plant.newPlant) {
			result = SetNewPlant(args, instr, sessionID);
		}
	}

	return result;
}

