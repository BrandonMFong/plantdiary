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
class Plant;

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
	int setEvent(const char * type, const BF::Time * eventTime, const char * eventUUID, const Entity * host, const BF::List<Entity *> * participants);

	int setNewPlant(const char * plantName, const char * plantUUID, const char * species, const BF::Time * birthDate, const char * userUUID);
	int modifyPlant(const char * plantUUID, const char * name, const char * species);

	int getPlantForUUID(const char * uuid, Plant ** plant);

PRIVATE:
	Database();
	~Database();

	void logException(sql::SQLException &e, const char * func);

  	sql::Connection * _connection;
  	sql::Driver * _driver;
};

#endif // SQLHANDLER_HPP
