/**
 * author: Brando
 * date: 1/30/23
 */

#ifndef INSTRUCTOR_HPP
#define INSTRUCTOR_HPP

#include "accessorspecifiers.hpp"
#include <internal/instructions.h>
#include <linux/limits.h>

/**
 * Object that represents a command sent from the cli
 *
 * This is an abstract class, so each different instruction needs
 * to have its own implementation
 */
class Instructor {
PUBLIC:

	/**
	 * Subclasses must not keep the instr memory when constructing
	 */
	static Instructor * create(PDInstruction * instr);

	virtual ~Instructor();

	/**
	 * This will be executed by the Driver class on a separate thread
	 *
	 * Subclasses are required to do as instructor as per the PDInstruction
	 *
	 * Each instructor is responsible for communicating with the cli
	 */
	virtual int execute() = 0;

	PDCommand command();

	/**
	 * The purpose of each value differs per instruction
	 */
	virtual const char subCommand();

	/**
	 * returns the instruction data
	 */
	void getData(char * data, short * length);

PROTECTED:
	Instructor(PDInstruction * instr);

PRIVATE:
	PDCommand _command;
	char _subCommand;
	short _length;
	unsigned char _data[kPDInstructionDataMaxLength];
};

#endif // INSTRUCTOR_HPP

