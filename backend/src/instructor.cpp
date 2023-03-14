/**
 * author: Brando
 * date: 1/30/23
 */

#include "instructor.hpp"
#include "instructorversion.hpp"
#include "instructorget.hpp"
#include "instructorset.hpp"
#include "instructorsession.hpp"
#include <stdlib.h>
#include <string.h>
#include <bflibcpp/bflibcpp.hpp>

Instructor * Instructor::create(PDInstruction * instr) {
	Instructor * result = NULL;

	if (instr != NULL) {
		PDCommand cmd;
		int error = PDInstructionGetCommand(instr, &cmd);

		if (error == 0) {
			switch (cmd) {
				case kPDCommandVersion:
					result = new InstructorVersion(instr);
					break;
				case kPDCommandGet:
					result = new InstructorGet(instr);
					break;
				case kPDCommandSet:
					result = new InstructorSet(instr);
					break;
				case kPDCommandSession:
					result = new InstructorSession(instr);
					break;
			}
		}
	}

	return result;
}

Instructor::Instructor(PDInstruction * instr) {
	if (PDInstructionGetCommand(instr, &this->_command) != 0) {
		BFDLog("Error getting command");
	}

	if (PDInstructionGetSubCommand(instr, &this->_subCommand) != 0) {
		BFDLog("Error getting subcommand");
	}

	if (memcpy(this->_data, instr->data, instr->length) == NULL) {
		BFDLog("couldn't copy data from instruction");
	} else {
		this->_length = instr->length;
	}
}

Instructor::~Instructor() {

}

void Instructor::getData(char * data, short * length) {
	if (data && length) {
		memcpy(data, this->_data, this->_length);
		*length = this->_length;
	}
}

PDCommand Instructor::command() {
	return this->_command;
}

const char Instructor::subCommand() {
	return this->_subCommand;
}

