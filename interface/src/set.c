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

void SetHelp() {
	printf("Argument : %s <sub commands>\n", kArgumentSet);
	printf("  sub commands\n");
	printf("    - '%s' : creates an event for specific plant\n", kArgumentSetEvent);
	printf("      - '%s' : specifies the type of event (%s, %s)\n", kArgumentSetEventType, kArgumentSetEventTypeWater, kArgumentSetEventTypeRepot);

}

int SetExec(Arguments * args) {
	BFDLog("Command: Set");
	PDInstruction i = {0};
	PDInstructionSetCommand(&i, kPDCommandSet);
	
	if (args->subCommand == kPDSubCommandSetEvent) {
		BFDLog("Sub Command: event");
		return SetEvent(args, &i);
	} else {
		BFDLog("Unknown sub command %d", args->subCommand);
	}

	return 0;
}

int SetEvent(Arguments * args, PDInstruction * instr) {
	int result = 0;
	char * eventTypes[2] = {kArgumentSetEventTypeWater, kArgumentSetEventTypeRepot};
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

	if (!BFArrayStringContainsString(eventTypes, sizeof(eventTypes) / sizeof(eventTypes[0]), args->type.set.eventType)) {
		BFDLog("Unknown event: %s", args->type.set.eventType);
		result = 51;
	}

	if (result == 0) {
		instr->length = snprintf(
			instr->data,
			kPDInstructionDataMaxLength,
			kPDJsonSetEvent,
			kPDKeySessionID,
			sessionID,
			kPDKeySetEventType,
			args->type.set.eventType,
			kPDKeySetEventCurrentTime,
			time(NULL)
		);

		BFDLog("data: %s", instr->data);
		result = FifoWrite(instr);
	}

	if (result == 0) {
		//result = FifoRead(&resp);
		BFDLog("%s", resp.data);
	}

	return result;
}

