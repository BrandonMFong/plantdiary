/**
 * author: Brando
 * date: 2/3/23
 */

#ifndef SESSION_H
#define SESSION_H

/**
 * Executes session command
 */
int SessionExec(int argc, char ** argv);

/// Help menu for session
void SessionHelp();

/**
 * Returns session id
 *
 * sessionID: nonnull
 */
int SessionGetSessionID(char * sessionID);

#endif // SESSION_H

