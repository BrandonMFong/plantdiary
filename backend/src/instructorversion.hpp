/**
 * author: Brando
 * date: 1/31/23
 */

#ifndef INSTRUCTORVERSION_HPP
#define INSTRUCTORVERSION_HPP

#include "accessorspecifiers.hpp"
#include "instructor.hpp"

class InstructorVersion : public Instructor {
	friend class Instructor;
PRIVATE:
	InstructorVersion(PDInstruction * instr);
	virtual ~InstructorVersion();
	int execute();
};

#endif // INSTRUCTORVERSION_HPP

