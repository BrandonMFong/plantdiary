/**
 * author: Brando
 * date: 2/3/23
 */

#include "instructorget.hpp"
#include "pipe.hpp"
#include <bflibcpp/bflibcpp.hpp>

InstructorGet::InstructorGet(PDInstruction * instr) : Instructor(instr) {

}

InstructorGet::~InstructorGet() {

}

int InstructorGet::execute() {
	char scmd = (char) this->subCommand();
	BFDLog("Command: get");
	BFDLog("Subcommand: %d", scmd);
	
	PDResponse response;
	strcpy(response.data, "8");
	response.length = strlen(response.data);
	Pipe::shared()->writeResponse(&response);

	return 0;
}

