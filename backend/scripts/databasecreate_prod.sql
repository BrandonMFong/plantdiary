USE plantdiary_prod;

source databasecreate.sql

INSERT INTO event_particpant_types (name, start_date) values 
("host", NOW()),
("members", NOW()),
("plants", NOW())
;

INSERT INTO event_types (name, start_date) values 
("water", NOW()),
("repot", NOW())
;

