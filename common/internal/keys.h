/**
 * author: Brando
 * date: 2/6/23
 */

#ifndef KEYS_H
#define KEYS_H

#define kPDKeyStdReturnMessageError "stderr"
#define kPDKeyStdReturnMessageErrorCode "code"
#define kPDKeyStdReturnMessageErrorDescription "description"

#define kPDKeyUsername "username"
#define kPDKeyPassword "password"
#define kPDKeySessionID "sessionid"

// Value will be an integer. 0 with no error
#define kPDKeySessionStop "error"

#define kPDKeySetEventType "type"

// Seconds since January 1, 1970
#define kPDKeySetEventCurrentTime "currtime"
#define kPDKeySetPlantBirthdate "birth"
#define kPDKeySetEventParticipantUUID "participantuuid"

#define kPDKeySetPlantName "name"
#define kPDKeySetPlantOption "option"
#define kPDKeySetPlantSpecies "species"
#define kPDKeySetPlantUUID "plantuuid"

#endif // KEYS_H

