#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdio.h>

// DIRS
#define ROOT_PATH ".achat_data/"
#define CHAT_PATH ".achat_data/chats/"

// FILES
#define ACTIVE_SESSIONS_FILE ".achat_data/active_sessions"
#define USER_DB ".achat_data/users.db"

// LIMITS

/* init the command handler
 * (check or create the file structure etc.)
 * @returns 0 on success -1 if there was an error
 */
int init_chat();

// login/out:
int login(char *args);
int register_user(char *args);

// create new chats/leave chats
int create_chat(char *args);

// send/recv msgs
int send(char *args);
int read_chat(char *args);

// search chats
int list(char *args);       // list all chats of an user
int list_users(char *args); // list all users
int search(char *args);     // search for a string in all chats returns chatnames + full message

// misc
int help();
int emojis();
int invalid_command(char *command);
#endif // COMMANDS_H
