/**
 * author: Brando
 * date: 4/5/23
 */

#ifndef NURSERY_HPP
#define NURSERY_HPP

#include <bflibcpp/accessorspecifiers.hpp>
class Nursery {
PUBLIC:

	static int initialize();
	static void deinitialize();
	static Nursery * shared();

PRIVATE:
	Nursery();
	~Nursery();
};

#endif // NURSERY_HPP

