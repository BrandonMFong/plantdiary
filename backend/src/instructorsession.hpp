/**
 * author: Brando
 * date: 2/3/23
 */

#ifndef INSTRUCTORSESSION_HPP
#define INSTRUCTORSESSION_HPP

#include "accessorspecifiers.hpp"
#include "instructor.hpp"

class InstructorSession : public Instructor {
	friend class Instructor;
PRIVATE:
	InstructorSession(PDInstruction * instr);
	virtual ~InstructorSession();
	int execute();
	int executeStart();
	int executeStatus();
	int executeStop();
};

#endif // INSTRUCTORSESSION_HPP

