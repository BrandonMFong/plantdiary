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
#include "internal/limits.h"
#include "internal/setplantflags.h"
#include "internal/values.h"

// App data path
#define kPDCommonAppDataPath "/tmp/plantdiary"

#define kPDCommonFifoFilePathInbound kPDCommonAppDataPath"/plantdiaryfifoinbound"
#define kPDCommonFifoFilePathOutbound kPDCommonAppDataPath"/plantdiaryfifooutbound"

#endif // PLANTDIARY_COMMON_H
