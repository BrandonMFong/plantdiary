/**
 * author: Brando
 * date: 2/3/23
 */

#ifndef INSTRUCTORGET_HPP
#define INSTRUCTORGET_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include "instructor.hpp"

class InstructorGet : public Instructor {
	friend class Instructor;

PRIVATE:
	InstructorGet(PDInstruction * instr);
	virtual ~InstructorGet();
	int execute();
};

#endif // INSTRUCTORGET_HPP

