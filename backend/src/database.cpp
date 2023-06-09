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
#include <bflibcpp/bflibcpp.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "user.hpp"
#include "logger.hpp"
#include "plant.hpp"

using namespace std;
using namespace BF;

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

int Database::copyPlantListForUserUUID(const char * userUUID, List<Plant *> * plants) {
	int result = 0;
	char q[512];
	const char * queryKeyPlantName = "plant_name";
	const char * queryKeyPlantSpecies = "plant_species";
	const char * queryKeyPlantBirthDate = "plant_birth_date";
	const char * queryKeyPlantDeathDate = "plant_death_date";
	const char * queryKeyPlantOwnershipStartDate = "start_date_ownership";
	sprintf(
		q, 
		"select p.name as '%s', p.species as '%s', unix_timestamp(p.birth_date) as '%s', unix_timestamp(p.death_date) as '%s', unix_timestamp(upb.start_date) as '%s' from plants as p join users_plants_bridge as upb on p.uuid = upb.plant_uuid where upb.user_uuid = '%s'",
		queryKeyPlantName,
		queryKeyPlantSpecies,
		queryKeyPlantBirthDate,
		queryKeyPlantDeathDate,
		queryKeyPlantOwnershipStartDate,
		userUUID
	);

	if (!plants) {
		result = 2;
	} else {
		try {
			sql::Statement * stmt = 0;
			sql::ResultSet * res = 0;

			BFDLog("Query: %s", q);
			stmt = this->_connection->createStatement();
			res = stmt->executeQuery(q); 
			while (res->next()) {
				int error = 0;
				Plant * p = Plant::createPlant(
					res->getString(queryKeyPlantName).c_str(),
					res->getString(queryKeyPlantSpecies).c_str(),
					(BFTime) res->getUInt(queryKeyPlantBirthDate),
					(BFTime) res->getUInt(queryKeyPlantDeathDate),
					(BFTime) res->getUInt(queryKeyPlantOwnershipStartDate),
					&error
				);

				if (error) {
					BFDLog("Error with creating user, %d", error);
				} else {
					result = plants->add(p);
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
				BFDLog("null result");
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
					BFDLog("Error with creating user, %d", error);
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

void GetStringToDateString(const Time * eventTime, char * buf) {
	sprintf(buf, "STR_TO_DATE('%02d/%02d/%02d %02d:%02d:%02d','%%d/%%m/%%Y %%H:%%i:%%s')",
			eventTime->day(), eventTime->month(), eventTime->year(),
			eventTime->hour(), eventTime->minute(), eventTime->second());
}

int Database::modifyPlant(const char * plantUUID, const char * name, const char * species) {
	int result = 0;
	size_t size = 2 << 8;
	char q[size];
	char plantName[kPDCommonPlantNameStringLength];
	char plantSpecies[kPDCommonPlantSpeciesStringLength];

	if (!plantUUID) {
		result = 15;
	} else {
		snprintf(q, size, "select * from plants where uuid = '%s'", plantUUID);

		BFDLog("Query: %s", q);
		try {
			sql::Statement * stmt = 0;
			sql::ResultSet * res = 0;

			stmt = this->_connection->createStatement();
			res = stmt->executeQuery(q); 
			if (!res->next()) {
				result = 18;
				BFDLog("null result");
			} else {
				if (name) {
					strcpy(plantName, name);
				} else {
					strcpy(plantName, res->getString("name").c_str());
				}

				if (species) {
					strcpy(plantSpecies, species);
				} else {
					strcpy(plantSpecies, res->getString("species").c_str());
				}
			}

			Delete(res);
			Delete(stmt);
		} catch (sql::SQLException &e) {
			result = 19;
			this->logException(e, __FUNCTION__);
		}
	}

	if (result == 0) {
		snprintf(q, size, "update plants set name = '%s', species = '%s' where uuid = '%s'", plantName, plantSpecies, plantUUID);

		BFDLog("Query: %s", q);
		
		try {
			sql::ResultSet * res = 0;
			sql::PreparedStatement * pstmt = NULL;

			pstmt = this->_connection->prepareStatement(q);
			res = pstmt->executeQuery(); 

			Delete(res);
			Delete(pstmt);
		} catch (sql::SQLException &e) {
			result = 1;
			this->logException(e, __FUNCTION__);
		}
	}


	return result;
}

int Database::setNewPlant(const char * plantName, const char * plantUUID, const char * species, const Time * birthDate, const char * userUUID) {
	int result = 0;
	size_t size = 2 << 8;
	char q[size];

	if (!plantName || !plantUUID || !userUUID || !species || !birthDate) {
		result = 11;
	} else {
		char buf[2 << 6];
		GetStringToDateString(birthDate, buf);

		snprintf(q, size, "insert into plants (uuid, name, species, birth_date, start_date) values ('%s', '%s', '%s', %s, NOW())", plantUUID, plantName, species, buf);

		BFDLog("Query: %s", q);
		
		try {
			sql::ResultSet * res = 0;
			sql::PreparedStatement * pstmt = NULL;

			pstmt = this->_connection->prepareStatement(q);
			res = pstmt->executeQuery(); 

			Delete(res);
			Delete(pstmt);
		} catch (sql::SQLException &e) {
			result = 1;
			this->logException(e, __FUNCTION__);
		}
	}

	if (result == 0) {
		snprintf(q, size, "insert into users_plants_bridge (user_uuid, plant_uuid, start_date) values ('%s', '%s', NOW())", userUUID, plantUUID);

		BFDLog("Query: %s", q);
		
		try {
			sql::ResultSet * res = 0;
			sql::PreparedStatement * pstmt = NULL;

			pstmt = this->_connection->prepareStatement(q);
			res = pstmt->executeQuery(); 

			Delete(res);
			Delete(pstmt);
		} catch (sql::SQLException &e) {
			result = 1;
			this->logException(e, __FUNCTION__);
		}

	}

	return result;
}

int Database::setEvent(
	const char * type,
	const Time * eventTime,
	const char * eventUUID,
	const Entity * host,
	const List<const Entity *> * participants
) {
	int result = 0;
	size_t size = 2 << 8;
	char q[size];

	if (!eventUUID || !type || !eventTime || !participants) {
		result = 2;
	} else {
		char buf[2 << 6];
		GetStringToDateString(eventTime, buf);
		snprintf(q, size, "insert into events (event_uuid, event_type_id, name, description, event_date, start_date) "
				"values ('%s', (select id from event_types where name = '%s'), 'Event: %s', "
				"'Type: %s, date: %02d/%02d/%02d', "
				"%s, NOW())", 
				eventUUID, type, type, type, eventTime->day(),
				eventTime->month(), eventTime->year(), buf);

		BFDLog("Query: %s", q);

		try {
			sql::ResultSet * res = 0;
			sql::PreparedStatement * pstmt = NULL;

			pstmt = this->_connection->prepareStatement(q);
			res = pstmt->executeQuery(); 

			Delete(res);
			Delete(pstmt);
		} catch (sql::SQLException &e) {
			result = 1;
			this->logException(e, __FUNCTION__);
		}
	}

	// insert the host	
	if (result == 0) {
		snprintf(q, size, "insert into event_participants "
				"(event_id, event_participant_type_id, entity_uuid) values "
				"((select id from events where event_uuid = '%s'), "
				"(select id from event_participant_types where name = 'host'), '%s')", 
				eventUUID, host->uuid());

		BFDLog("Query: %s", q);
		
		try {
			sql::ResultSet * res = 0;
			sql::PreparedStatement * pstmt = NULL;

			pstmt = this->_connection->prepareStatement(q);
			res = pstmt->executeQuery(); 

			Delete(res);
			Delete(pstmt);
		} catch (sql::SQLException &e) {
			result = 4;
			this->logException(e, __FUNCTION__);
		}
	}

	// insert the participants
	if (result == 0) {
		char participantType[2 << 4];
		const List<const Entity *>::Node * n = participants->first();
		for (; n != NULL; n = n->next()) {
			const Entity * e = n->object();

			if (!e) {
				result = 2;
				break;
			} else {
				switch (e->type()) {
					case Entity::Type::kEntityTypeUser:
						strcpy(participantType, "members");
						break;
					case Entity::Type::kEntityTypePlant:
						strcpy(participantType, "plants");
						break;
					default:
						result = 3;
						break;
				}
			}

			if (result) {
				break; // break if error occurred
			} else {
				snprintf(q, size, "insert into event_participants "
						"(event_id, event_participant_type_id, entity_uuid) values "
						"((select id from events where event_uuid = '%s'), "
						"(select id from event_participant_types where name = '%s'), '%s')", 
						eventUUID, participantType, e->uuid());

				BFDLog("Query: %s", q);
				
				try {
					sql::ResultSet * res = 0;
					sql::PreparedStatement * pstmt = NULL;

					pstmt = this->_connection->prepareStatement(q);
					res = pstmt->executeQuery(); 

					Delete(res);
					Delete(pstmt);
				} catch (sql::SQLException &e) {
					result = 1;
					this->logException(e, __FUNCTION__);
				}
			}
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
					BFDLog("Error with creating user, %d", error);
				} else {
					error = users->add(u);

					if (error) {
						BFDLog("Errorrrr with adding user to list %d", error);
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
	BFErrorPrint("Experienced an expection in sql handler");
	BFErrorPrint("Function: %s", func);
	BFErrorPrint("Error: %s", e.what());
	BFErrorPrint("Code: %d", e.getErrorCode());
	BFErrorPrint("State: %s", e.getSQLStateCStr());
}

