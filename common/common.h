/**
 * author: Brando
 * date: 1/13/23
 */

#ifndef PLANTDIARY_COMMON_H
#define PLANTDIARY_COMMON_H

#include "internal/instructions.h"
#include "internal/responses.h"
#include "internal/json.h"
#include "internal/keys.h"

#define kPDCommonFifoFilePathInbound "/tmp/plantdiaryfifoinbound"
#define kPDCommonFifoFilePathOutbound "/tmp/plantdiaryfifooutbound"

#define kPDCommonUsernameMaxLength 2 << 7
#define kPDCommonPasswordMaxLength 2 << 7

#endif // PLANTDIARY_COMMON_H
