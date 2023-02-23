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
#include "interface.h"
#include <fcntl.h>

#define kSessionSessionIDFilename "sessionid"

int SessionStart(Arguments * args);
int SessionStatus(Arguments * args);
int SessionStop(Arguments * args);

void SessionHelp() {
	printf("Argument %s :\n", kArgumentSession);
	printf("  sub commands\n");
	printf("    - %s :\tstarts session for user.  Will return a\n", kArgumentSessionStart);
	printf("\t\tsession id.  User must keep a record of it\n");
	printf("    - %s : checks session status for user\n", kArgumentSessionStatus);
	printf("    - %s : stops session for current active user\n", kArgumentSessionStop);
}

int SessionExec(Arguments * args) {
	int result = 0;

	if (args->subCommand == kPDSubCommandSessionStart) {
		result = SessionStart(args);
	} else if (args->subCommand == kPDSubCommandSessionStatus) {
		result = SessionStatus(args);
	} else if (args->subCommand == kPDSubCommandSessionStop) {
		result = SessionStop(args);
	} else {
		result = 41;
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

int SessionClearSessionID() {
	char sessionIDPath[PATH_MAX];
	int result = InterfaceGetUserDataPath(sessionIDPath);
	int fd = 0;

	if (result == 0) {
		sprintf(sessionIDPath, "%s/%s", sessionIDPath, kSessionSessionIDFilename);
		remove(sessionIDPath);
	}
	
	return 0;
}

int SessionGetSessionID(char * sessionID) {
	char sessionIDPath[PATH_MAX];
	int result = InterfaceGetUserDataPath(sessionIDPath);
	int fd = 0;

	if (result == 0) {
		sprintf(sessionIDPath, "%s/%s", sessionIDPath, kSessionSessionIDFilename);
		fd = open(sessionIDPath, O_RDONLY);

		if (fd == -1) {
			Error("Could not open '%s'", sessionIDPath);
			result = 40;
		}
	}

	if (result == 0) {
		if (read(fd, sessionID, UUID_STR_LEN + 1) == -1) {
			Error("Could not save session id");
			result = 41;
		}
	}

	close(fd);

	return 0;
}

int SessionSaveSessionID(const char * sessionID) {
	char sessionIDPath[PATH_MAX];
	int result = InterfaceGetUserDataPath(sessionIDPath);
	int fd = 0;

	if (result == 0) {
		sprintf(sessionIDPath, "%s/%s", sessionIDPath, kSessionSessionIDFilename);
		fd = open(sessionIDPath, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR);

		if (fd == -1) {
			Error("Could not open '%s'", sessionIDPath);
			result = 40;
		}
	}

	if (result == 0) {
		if (write(fd, sessionID, strlen(sessionID) + 1) == -1) {
			Error("Could not save session id");
			result = 41;
		}
	}

	close(fd);

	return 0;
}

int SessionStart(Arguments * args) {
	int result = 0;
	char userName[kPDCommonUsernameMaxLength];
	char password[kPDCommonPasswordMaxLength];
	char sessionID[UUID_STR_LEN];
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
            strcpy(sessionID, val->u.object.values[0].value->u.string.ptr);
			DLog("Session id: %s", sessionID);

			if (args->print) {
				printf("%s\n", sessionID);
			} else {
				result = SessionSaveSessionID(sessionID);
			}
		}
	}

	json_value_free(val);
	
	return result;
}

int SessionStatus(Arguments * args) {
	int result = 0;
	char sessionID[UUID_STR_LEN];
	PDInstruction instr = {0};
	PDResponse resp = {0};
	json_value * val = NULL;
	char username[kPDCommonUsernameMaxLength];

	if (strlen(args->sessionID) == 0) {
		// If we couldn't find the session id in the arguments then we
		// will look at the cache
		result = SessionGetSessionID(sessionID);
		if (result) {
			Error("Please provide session ID");
		}
	} else {
		strcpy(sessionID, args->sessionID);
	}

	if (result == 0) {
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
			for (int i = 0; i < val->u.object.length; i++) {
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

int SessionStop(Arguments * args) {
	int result = 0;
	char sessionID[UUID_STR_LEN];
	PDInstruction instr = {0};
	PDResponse resp = {0};
	json_value * val = NULL;

	if (strlen(args->sessionID) == 0) {
		// If we couldn't find the session id in the arguments then we
		// will look at the cache
		result = SessionGetSessionID(sessionID);
		if (result) {
			Error("Please provide session ID");
		}

	} else {
		strcpy(sessionID, args->sessionID);
	}

	if (result == 0) {
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

	// Clean up the session id cache
	if (result == 0) {
		result = SessionClearSessionID();
	}

	return 0;
}

