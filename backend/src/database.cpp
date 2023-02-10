/**
 * author: Brando
 * date: 12/21/22
 *
 * https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-examples-complete-example-1.html
 *
 * documentation: https://dev.mysql.com/doc/connector-cpp/1.1/en/
 *
 */

#include "database.hpp"
#include <cpplib.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cpplib.hpp>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "user.hpp"
#include "logger.hpp"

using namespace std;

Database * gSqlHandler = 0;

int Database::initialize() {
	int result = 0;
	gSqlHandler = new Database;

	if (gSqlHandler == NULL) {
		result = 1;
	} else {
		Logger::shared()->writeString("Database initialized");
	}

	return result;
}

void Database::deinitialize() {
	Delete(gSqlHandler);
}

Database * Database::shared() {
	return gSqlHandler;
}

Database::Database() {
	try {
		/* Create a connection */
		this->_driver = get_driver_instance();
		this->_connection = this->_driver->connect("localhost", "plantdiary_test_user1", "password");
		/* Connect to the MySQL test database */
		this->_connection->setSchema(kDatabasePlantDiaryName);
	} catch (sql::SQLException & e) {
		this->logException(e, __FUNCTION__);
	}
}

Database::~Database() {
	this->_driver = NULL;
	Delete(this->_connection);
}

int Database::getUserForCredentials(const char * username, const char * hash, User ** user) {
	int result = 0;
	char q[512];
	sprintf(q, "select u.uuid, u.username, u.first_name, u.last_name, up.password_hash from users as u join users_passwords as up on u.uuid = up.user_uuid where up.end_date is null and u.end_date is null and up.password_hash = \"%s\" and u.username = \"%s\"", hash, username);

	if (!user) {
		result = 2;
	} else {
		try {
			sql::Statement * stmt = 0;
			sql::ResultSet * res = 0;

			stmt = this->_connection->createStatement();
			res = stmt->executeQuery(q); 
			if (!res->next()) {
				result = 3;
				DLog("null result");
			} else {
				int error = 0;
				User * u = User::createUser(
					res->getString("uuid").c_str(),
					res->getString("username").c_str(),
					res->getString("first_name").c_str(),
					res->getString("last_name").c_str(),
					&error
				);

				if (error) {
					DLog("Error with creating user, %d", error);
				} else {
					*user = u;
				}
			}

			Delete(res);
			Delete(stmt);
		} catch (sql::SQLException &e) {
			result = 1;
			this->logException(e, __FUNCTION__);
		}
	}



	return result;
}

/*
int Database::createUsers(List<User *> * users) {
	int result = 0;
	const char * q = "SELECT * from users";

	if (!users) {
		result = 2;
	} else {
		try {
			sql::Statement * stmt = 0;
			sql::ResultSet * res = 0;

			stmt = this->_connection->createStatement();
			res = stmt->executeQuery(q); 
			while (res->next()) {
				int error = 0;
				User * u = User::createUser(
					res->getString("uuid").c_str(),
					res->getString("first_name").c_str(),
					res->getString("last_name").c_str(),
					&error
				);

				if (error) {
					DLog("Error with creating user, %d", error);
				} else {
					error = users->add(u);

					if (error) {
						DLog("Errorrrr with adding user to list %d", error);
					}
				}
			}

			Delete(res);
			Delete(stmt);
		} catch (sql::SQLException &e) {
			result = 1;
			this->logException(e, __FUNCTION__);
		}
	}

	return result;
}
*/
void Database::logException(sql::SQLException &e, const char * func) {
	Error("Experienced an expection in sql handler");
	Error("Function: %s", func);
	Error("Error: %s", e.what());
	Error("Code: %d", e.getErrorCode());
	Error("State: %s", e.getSQLStateCStr());
}

