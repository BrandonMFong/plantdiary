/**
author: Brando
date: 12/22/22
*/

-- user table
CREATE TABLE IF NOT EXISTS users(
	id INT AUTO_INCREMENT PRIMARY KEY,
	username VARCHAR(255) NOT NULL,
	uuid VARCHAR(255) NOT NULL,
	first_name VARCHAR(255) NOT NULL,
	last_name VARCHAR(255),
	start_date DATETIME NOT NULL,
	end_date DATETIME
);

-- user password table
CREATE TABLE IF NOT EXISTS users_passwords(
	id INT AUTO_INCREMENT PRIMARY KEY,
	user_uuid VARCHAR(255) NOT NULL,
	password_hash VARCHAR(255) NOT NULL,
	start_date DATETIME NOT NULL,
	end_date DATETIME
);

-- all available plants
CREATE TABLE IF NOT EXISTS plant_types(
	id INT AUTO_INCREMENT PRIMARY KEY,
	user_uuid VARCHAR(255) NOT NULL,
	name VARCHAR(255) NOT NULL,
	start_date DATETIME,
	end_date DATETIME
);

-- all actual plants users own
CREATE TABLE IF NOT EXISTS plants(
	id INT AUTO_INCREMENT PRIMARY KEY,
	uuid VARCHAR(255) NOT NULL,
	name VARCHAR(255) NOT NULL,
	plant_type_id INT,
	birth_date DATETIME,
	death_date DATETIME
);

-- ownership table
CREATE TABLE IF NOT EXISTS users_plants_bridge(
	id INT AUTO_INCREMENT PRIMARY KEY,
	user_uuid VARCHAR(255) NOT NULL,
	plant_uuid VARCHAR(255) NOT NULL,
	start_date DATETIME,
	end_date DATETIME
);

-- events
CREATE TABLE IF NOT EXISTS events(
	id INT AUTO_INCREMENT PRIMARY KEY,
	event_type_id INT NOT NULL,
	event_uuid VARCHAR(255) NOT NULL,
	name VARCHAR(255),
	description VARCHAR(255),
	event_date DATETIME NOT NULL,
	start_date DATETIME NOT NULL,
	end_date DATETIME
);

-- event types
CREATE TABLE IF NOT EXISTS event_types(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(255),
	start_date DATETIME,
	end_date DATETIME
);

-- event participants
CREATE TABLE IF NOT EXISTS event_participants(
	id INT AUTO_INCREMENT PRIMARY KEY,
	event_id INT,
	event_participant_type_id INT,
	entity_uuid VARCHAR(255) /* Can be both user and plant */
);

-- event particpant types
CREATE TABLE IF NOT EXISTS event_participant_types(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(255),
	start_date DATETIME,
	end_date DATETIME
);

INSERT INTO event_participant_types (name, start_date) values 
("host", NOW()),
("members", NOW()),
("plants", NOW())
;

INSERT INTO event_types (name, start_date) values 
("userlogin", NOW()),
("plantwater", NOW()),
("plantrepot", NOW())
;

