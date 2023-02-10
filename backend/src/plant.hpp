/**
 * author: Brando
 * date: 11/29/22
 *
 */

#ifndef PLANT_HPP
#define PLANT_HPP

#include "accessorspecifiers.hpp"
#include <uuid/uuid.h>

#define kPlantNameLength 2 << 7

class Plant {
PUBLIC:

PRIVATE:
	Plant();
	~Plant();

	char _name[kPlantNameLength];
	char _uuid[UUID_STR_LEN];
};

#endif // PLANT_HPP

