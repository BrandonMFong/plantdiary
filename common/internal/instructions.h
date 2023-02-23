/**
 * author: Brando
 * date: 1/30/23
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <linux/limits.h>

typedef enum {
	///
	kPDCommandUnknown = -1,

	/// Returns version number
	kPDCommandVersion = 0,

	/// Fetch command
	kPDCommandGet = 1,

	/// Session management
	kPDCommandSession = 2,

	kPDCommandInit = 3,

	kPDCommandHelp = 4,

	// max is 0xF
} PDCommand;

/// Sub Commands for Get
#define kPDSubCommandGetPlantCount 0

/// Sub Commands for Session
/// If user already exists, the current session id will be returned
#define kPDSubCommandSessionStart 0
#define kPDSubCommandSessionStatus 1
#define kPDSubCommandSessionStop 2

/// Sub commands for version
#define kPDSubCommandVersionGetDaemon 0
#define kPDSubCommandVersionGetAll 1

// 4096
#define kPDInstructionDataMaxLength PIPE_BUF - sizeof(char) - sizeof(short)

/**
 * PIPE buffer limit is 4096
 */
typedef struct {
	/**
	 * 4 bits : command
	 * 4 bits : sub command
	 */
	unsigned char command;

	/**
	 * 4 bits : reserved
	 * 12 bits : data length
	 */
	unsigned short length;

	char data[kPDInstructionDataMaxLength];
} PDInstruction;

static void PDInstructionSetCommand(PDInstruction * instr, PDCommand command) {
	if (instr) {
		instr->command |= (command << 4);
	}
}

static void PDInstructionSetSubCommand(PDInstruction * instr, const char scmd) {
	if (instr) {
		instr->command |= (scmd & 0x0F);
	}
}

static int PDInstructionGetCommand(PDInstruction * instr, PDCommand * cmd) {
	if (instr && cmd) {
		*cmd = (PDCommand) (instr->command >> 4);
		return 0;
	}
	return -1;
}

static int PDInstructionGetSubCommand(PDInstruction * instr, char * scmd) {
	if (instr && scmd) {
		*scmd = 0x0F & instr->command;
		return 0;
	}
	return -1;
}

#endif // INSTRUCTIONS_H

