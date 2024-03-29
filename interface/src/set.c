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
	printf("      - '%s' <string> : uuid of the participant affected by event\n", kArgumentSetEventParticipantUUID);
	printf("    - '%s' : creates/modifies plant\n", kArgumentSetPlant);
	printf("      - [ '%s' <string> ] : Plant name\n", kArgumentSetPlantName);
	printf("      - [ '%s' <string> ] : Plant species.  Can be any string.  Treated as a label\n", kArgumentSetPlantSpecies);
	printf("      - '%s' <string> : Plant uuid\n", kArgumentSetPlantUUID);
	printf("      - '%s' | '%s' : Creates new or modifies plant\n", kArgumentSetPlantNew, kArgumentSetPlantModify);

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
	} else if (strlen(args->type.set.event.participantUUID) == 0) {
		result = 57;
		BFErrorPrint("Please provide participant uuid");
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
			BFTimeGetCurrentTime(),
			kPDKeySetEventParticipantUUID,
			args->type.set.event.participantUUID
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
			kPDKeySetPlantOption,
			kPDSetPlantOptionNew,
			kPDKeySetPlantBirthdate,
			BFTimeGetCurrentTime(),
			kPDKeySetPlantSpecies,
			args->type.set.plant.species
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


int SetModifyPlant(Arguments * args, PDInstruction * instr, char * sessionID) {
	int result = 0;
	PDResponse resp = {0};
	
	// Send data
	instr->length = snprintf(
		instr->data,
		kPDInstructionDataMaxLength,
		kPDJsonSetModifyPlant,
		kPDKeySetPlantOption,
		kPDSetPlantOptionModify,
		kPDKeySessionID,
		sessionID,
		kPDKeySetPlantUUID,
		args->type.set.plant.uuid,
		kPDKeySetPlantName,
		args->type.set.plant.name,
		kPDKeySetPlantSpecies,
		args->type.set.plant.species
	);

	BFDLog("data: %s", instr->data);
	result = FifoWrite(instr);

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
		switch (args->type.set.plant.option) {
		case kPDSetPlantOptionNew:
			result = SetNewPlant(args, instr, sessionID);
			break;
		case kPDSetPlantOptionModify:
			result = SetModifyPlant(args, instr, sessionID);
			break;
		default:
			BFErrorPrint("Unkown option: %d", args->type.set.plant.option);
			break;
		}
	}

	return result;
}

