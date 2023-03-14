/**
 * author: Brando
 * date: 1/31/23
 */

#include "instructorversion.hpp"
#include "pipe.hpp"
#include <stdio.h>
#include <string.h>
#include <bflibcpp/bflibcpp.hpp>
#include <common.h>
#include "meta.h"

InstructorVersion::InstructorVersion(PDInstruction * instr) : Instructor(instr) {

}

InstructorVersion::~InstructorVersion() {

}

int InstructorVersion::execute() {
	PDResponse response;
	strcpy(response.data, kVersionString);
	response.length = strlen(response.data);
	Pipe::shared()->writeResponse(&response);
	BFDLog("Wrote response successfully");
	return 0;
}

