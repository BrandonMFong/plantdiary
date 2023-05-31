/**
 * author: Brando
 * date: 2/3/23
 *
 * This should hold some formats for us
 */

#ifndef INTERNAL_JSON_H
#define INTERNAL_JSON_H

#define kPDJsonSessionStart "{\"%s\" : \"%s\", \"%s\" : \"%x\"}"
#define kPDJsonSessionID "{\"%s\" : \"%s\"}"
#define kPDJsonSessionStatus kPDJsonSessionStart
#define kPDJsonSessionStop "{\"%s\" : %d}"
#define kPDJsonSetEvent "{\"%s\" : \"%s\", \"%s\" : \"%s\", \"%s\" : %d, \"%s\" : \"%s\"}"
#define kPDJsonSetNewPlant "{\"%s\" : \"%s\", \"%s\" : \"%s\", \"%s\" : %d, \"%s\" : %d, \"%s\" : \"%s\"}"
#define kPDJsonSetModifyPlant "{\"%s\" : %d, \"%s\" : \"%s\", \"%s\" : \"%s\", \"%s\" : \"%s\", \"%s\" : \"%s\"}"

#endif // INTERNAL_JSON_H

