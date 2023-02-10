/**
 * author: Brando
 * date: 2/1/23
 */

#ifndef RESPONSES_H
#define RESPONSES_H

#include <linux/limits.h>

#define kPDResponseDataMaxLength PIPE_BUF - sizeof(short)

typedef struct {
	short length;
	char data[kPDResponseDataMaxLength];
} PDResponse;

#endif // RESPONSES_H

