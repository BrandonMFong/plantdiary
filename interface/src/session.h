/**
 * author: Brando
 * date: 2/3/23
 */

#ifndef SESSION_H
#define SESSION_H

#include "typearguments.h"

/**
 * Executes session command
 */
int SessionExec(Arguments * args);

/// Help menu for session
void SessionHelp();

/**
 * Returns session id
 *
 * sessionID: nonnull
 */
int SessionGetSessionID(char * sessionID);

#endif // SESSION_H

