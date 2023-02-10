/**
 * author: Brando
 * date: 2/1/23
 */

#ifndef FIFO_H
#define FIFO_H

#include <internal/instructions.h>
#include <internal/responses.h>

int FifoWrite(PDInstruction * instr);

/**
 * This will poll the readFile until plantdairy sends a response back
 */
int FifoRead(PDResponse * response);

#endif // FIFO_H
