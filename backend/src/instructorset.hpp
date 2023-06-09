/**
 * author: Brando
 * date: 3/8//23
 */

#ifndef INSTRUCTORSET_HPP
#define INSTRUCTORSET_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include "instructor.hpp"

class InstructorSet : public Instructor {
	friend class Instructor;

PRIVATE:
	InstructorSet(PDInstruction * instr);
	virtual ~InstructorSet();
	int execute();

	/**
	 * executes the event sub command for set
	 *
	 * This writes into database
	 */
	int executeEvent();

	/**
	 * Modifies or adds plants
	 */
	int executePlant();
};

#endif // INSTRUCTORSET_HPP

