/**
 * author: Brando
 * date: 2/3/23
 */

#include "session.h"
#include "arguments.h"
#include "fifo.h"
#include <stdio.h>
#include <string.h>
#include <common.h>
#include <unistd.h>
#include <termios.h>
#include <internal/responses.h>
#include <internal/json.h>
#include <internal/keys.h>
#include <clib.h>
#include <external/json-parser/json.h>
#include <uuid/uuid.h>
#include "print.h"

int SessionStart();
int SessionStatus(int argc, char ** argv);
int SessionStop(int argc, char ** argv);

void SessionHelp() {
	printf("Argument %s :\n", kArgumentSession);
	printf("  sub commands\n");
	printf("    - %s :\tstarts session for user.  Will return a\n", kArgumentSessionStart);
	printf("\t\tsession id.  User must keep a record of it\n");
	printf("    - %s : checks session status for user\n", kArgumentSessionStatus);
	printf("    - %s : stops session for current active user\n", kArgumentSessionStop);
}

int SessionExec(int argc, char ** argv) {
	int result = 0;

	if (argc < 3) {
		printf("Please provide a subcommand for arg '%s'\n", argv[1]);
		result = 40;
	} else {
		if (!strcmp(argv[2], kArgumentSessionStart)) {
			result = SessionStart();
		} else if (!strcmp(argv[2], kArgumentSessionStatus)) {
			result = SessionStatus(argc, argv);
		} else if (!strcmp(argv[2], kArgumentSessionStop)) {
			result = SessionStop(argc, argv);
		} else {
			print("Unknown argument: %s\n", argv[2]);
			result = 41;
		}
	}

	return result;
}

void GetPassword(char password[]) {
    static struct termios oldt, newt;
    int i = 0;
    int c;

    /*saving the old settings of STDIN_FILENO and copy settings for resetting*/
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    /*setting the approriate bit in the termios struct*/
    newt.c_lflag &= ~(ECHO);          

    /*setting the new bits*/
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*reading the password from the console*/
    while ((c = getchar())!= '\n' && c != EOF && i < kPDCommonPasswordMaxLength){
        password[i++] = c;
    }
    password[i] = '\0';

    /*resetting our old STDIN_FILENO*/ 
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

	printf("\n");
}

/**
 * djb2 hash function
 */
unsigned long hash(unsigned char * str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

int SessionStart() {
	int result = 0;
	char userName[kPDCommonUsernameMaxLength];
	char password[kPDCommonPasswordMaxLength];
	PDResponse resp;
	json_value * val = NULL;

	// Get user name
	printf("Enter username: ");
	if (fgets(userName, kPDCommonUsernameMaxLength, stdin) == NULL) {
		result = 1;
	} else {
		userName[strlen(userName) - 1] = '\0';
	}

	// get password without echoing print
	if (result == 0) {
		printf("Enter password: ");
		GetPassword(password);
		unsigned long h = hash(password);
		DLog("hash: 0x%x", h);

		PDInstruction instr = {0};
		PDInstructionSetCommand(&instr, kPDCommandSession);
		PDInstructionSetSubCommand(&instr, kPDSubCommandSessionStart);
		instr.length = snprintf(
			instr.data,
			kPDInstructionDataMaxLength,
			kPDJsonSessionStart,
			kPDKeyUsername,
			userName,
			kPDKeyPassword,
			h
		);

		DLog("json: %s", instr.data);
		DLog("json length: %d", instr.length);

		result = FifoWrite(&instr);
	}

	if (result == 0) {
		result = FifoRead(&resp);
		DLog("%s", resp.data);
	}

	if (result == 0) {
		val = json_parse(resp.data, resp.length);
		if (val == NULL) {
			result = 8;
			DLog("Could not parse json data");
		}
	}

	if (result == 0) {
		if (val->u.object.length != 1) {
			result = 9;
			DLog("error with json data, length is %d", val->u.object.length);
		} else if (strcmp(val->u.object.values[0].name, kPDKeySessionID)) {
			DLog("Incorrect key for session start: %s", val->u.object.values[0].name);
			result = 10;
		} else {
            printf("Session ID: %s\n", val->u.object.values[0].value->u.string.ptr);
		}
	}

	json_value_free(val);
	
	return result;
}

int SessionStatus(int argc, char ** argv) {
	int result = 0;
	char sessionID[UUID_STR_LEN];
	PDInstruction instr = {0};
	PDResponse resp = {0};
	json_value * val = NULL;
	char username[kPDCommonUsernameMaxLength];

	int i = IndexOfStringInArray(argv, argc, kArgumentSessionID);
	if (i == -1) {
		Error("Please provide session ID");
		result = 23;
	} else if (i >= argc) {
		Error("a problem occurred");
		DLog("index is out of range");
		result = 24;
	} else {
		strcpy(sessionID, argv[i + 1]);
		PDInstructionSetCommand(&instr, kPDCommandSession);
		PDInstructionSetSubCommand(&instr, kPDSubCommandSessionStatus);
		instr.length = snprintf(
			instr.data,
			kPDInstructionDataMaxLength,
			kPDJsonSessionID,
			kPDKeySessionID,
			sessionID
		);

		DLog("%s\n", instr.data);
		result = FifoWrite(&instr);
	}

	if (result == 0) {
		result = FifoRead(&resp);
	}

	if (result == 0) {
		DLog("%s", resp.data);
		val = json_parse(resp.data, resp.length);
		if (val == NULL) {
			DLog("json data null");
			result = 26;
		} else if (val->u.object.length != 2) {
			DLog("length is %d", val->u.object.length);
			result = 27;
		} else {
			username[0] = '\0';
			sessionID[0] = '\0';
			for (i = 0; i < val->u.object.length; i++) {
				if (!strcmp(val->u.object.values[i].name, kPDKeyUsername)) {
					strcpy(username, val->u.object.values[i].value->u.string.ptr);
				} else if (!strcmp(val->u.object.values[i].name, kPDKeySessionID)) {
					strcpy(sessionID, val->u.object.values[i].value->u.string.ptr);
				} 
			}

			if (!strlen(username) && !strlen(sessionID)) {
				printf("Status: inactive\n");
			} else {
				printf("Status: active\n");
				printf("user: %s\n", username);
			}
		}
	}

	json_value_free(val);

	return 0;
}

int SessionStop(int argc, char ** argv) {
	int result = 0;
	char sessionID[UUID_STR_LEN];
	PDInstruction instr = {0};
	PDResponse resp = {0};
	json_value * val = NULL;

	int i = IndexOfStringInArray(argv, argc, kArgumentSessionID);
	if (i == -1) {
		Error("Please provide session ID");
		result = 29;
	} else if (i >= argc) {
		Error("a problem occurred");
		DLog("index is out of range");
		result = 23;
	} else {
		strcpy(sessionID, argv[i + 1]);
		PDInstructionSetCommand(&instr, kPDCommandSession);
		PDInstructionSetSubCommand(&instr, kPDSubCommandSessionStop);
		instr.length = snprintf(
			instr.data,
			kPDInstructionDataMaxLength,
			kPDJsonSessionID,
			kPDKeySessionID,
			sessionID
		);

		DLog("%s\n", instr.data);
		result = FifoWrite(&instr);
	}

	if (result == 0) {
		result = FifoRead(&resp);
	}

	if (result == 0) {
		DLog("response: %s", resp.data);
		val = json_parse(resp.data, resp.length);
		if (val == NULL) {
			result = 36;
		} else if (val->u.object.length != 1) {
			result = 34;
			DLog("error with json data, length is %d", val->u.object.length);
		} else if (strcmp(val->u.object.values[0].name, kPDKeySessionStop)) {
			DLog("Incorrect key for session start: %s", val->u.object.values[0].name);
			result = 35;
		} else {
            DLog("Error code: %d", val->u.object.values[0].value->u.integer);
            result = val->u.object.values[0].value->u.integer;

			if (result == 0) {
				printf("Session stopped\n");
			} else {
				Error("Could not stop session");
			}
		}
	}

	return 0;
}

