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
#include "internal/setevents.h"

// App data path
#define kPDCommonAppDataPath "/tmp/plantdiary"

#define kPDCommonFifoFilePathInbound kPDCommonAppDataPath"/plantdiaryfifoinbound"
#define kPDCommonFifoFilePathOutbound kPDCommonAppDataPath"/plantdiaryfifooutbound"

#define kPDCommonUsernameMaxLength 2 << 7
#define kPDCommonPasswordMaxLength 2 << 7

#define kPDCommonEventTypeStringLength 2 << 5
#define kPDCommonPlantNameStringLength 2 << 5

#endif // PLANTDIARY_COMMON_H
