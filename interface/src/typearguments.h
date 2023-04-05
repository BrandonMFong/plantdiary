/**
 * author: Brando
 * date: 2/20/23
 */

#ifndef TYPEARGUMENTS_H
#define TYPEARGUMENTS_H

#include <internal/instructions.h>
#include <uuid/uuid.h>
#include <stdbool.h>
#include <common.h>

/**
 * This will represent the command line arguments
 */
typedef struct {
	char appName[2 << 4];
	PDCommand command;

	/// Initialized to -1
	char subCommand;
	char sessionID[UUID_STR_LEN + 1];

	union Type {
		// Session
		struct {
			bool print;
		} session;

		// Help
		struct {
			/// The command that the user wants to get help with
			char helpPDCommandArg[2 << 5];
		} help;

		// Set
		struct {
			struct {
				char type[kPDCommonEventTypeStringLength];
			} event;

			struct {
				bool newPlant;
			} plant;
		} set;
	} type;
} Arguments;

#endif // TYPEARGUMENTS_H

