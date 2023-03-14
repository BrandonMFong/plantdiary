USE plantdiary_test

source databasecreate_prod.sql

-- Insert test data

SET @user1 = UUID();
SET @user2 = UUID();
SET @user3 = UUID();
SET @user4 = UUID();
SET @user5 = UUID();
INSERT INTO users (uuid, username, first_name, last_name, start_date) values 
(@user1, "alexabraham", "Alex", "Abraham", NOW()),
(@user2, "brianbeckam", "Brian", "Beckam", NOW()),
(@user3, "charlescooper", "Charles", "Cooper", NOW()),
(@user4, "derrickdaniels", "Derrick", "Daniels", NOW()),
(@user5, "emanualerickson", "Emanuel", "Erickson", NOW())
;

INSERT INTO users_passwords (user_uuid, password_hash, start_date) values
(@user1, "7c78c509", NOW()),
(@user2, "7c7955cd", NOW()),
(@user3, "7c79e691", NOW()),
(@user4, "7c7a7755", NOW()),
(@user5, "7c7b0819", NOW())
;

SET @plant1 = UUID();
SET @plant2 = UUID();
SET @plant3 = UUID();
SET @plant4 = UUID();
SET @plant5 = UUID();
INSERT INTO plants (uuid, name, birth_date) values
(@plant1, "Alice", NOW()),
(@plant2, "Buster", NOW()),
(@plant3, "Clams", NOW()),
(@plant4, "Dusty", NOW()),
(@plant5, "Easter", NOW())
;

