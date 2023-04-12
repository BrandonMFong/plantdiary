/**
 * author: Brando
 * date: 2/1/23
 */

#ifndef RESPONSES_H
#define RESPONSES_H

#include <linux/limits.h>

#define kPDResponseDataMaxLength PIPE_BUF - sizeof(short) - sizeof(char)

typedef struct {
	unsigned char returnCode;
	short length;
	char data[kPDResponseDataMaxLength];
} PDResponse;

#endif // RESPONSES_H

