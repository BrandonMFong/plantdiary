/**
 * author: Brando
 * date: 12/21/22
 *
 */

#ifndef SQLHANDLER_HPP
#define SQLHANDLER_HPP

#include <bflibcpp/accessorspecifiers.hpp>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <bflibcpp/list.hpp>
#include <bflibcpp/time.hpp>
#include "entity.hpp"

#define DATABASE_UNDER_TEST 1

#if DATABASE_UNDER_TEST == 1
#define kDatabasePlantDiaryName "plantdiary_test"
#else
#define kDatabasePlantDiaryName "plantdiary_prod"
#endif

class User;

/**
 *
 */
class Database {
PUBLIC:
	/**
	 * Creates shared handler
	 */
	static int initialize();

	static void deinitialize();
	static Database * shared();

	int getUserForCredentials(const char * username, const char * hash, User ** user);

	/**
	 * Inserts an entry to the events table
	 */
	int saveEvent(const char * type, const BF::Time * eventTime, const BF::List<Entity *> * participants);

PRIVATE:
	Database();
	~Database();

	void logException(sql::SQLException &e, const char * func);

  	sql::Connection * _connection;
  	sql::Driver * _driver;
};

#endif // SQLHANDLER_HPP
