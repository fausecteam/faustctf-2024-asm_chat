#ifndef CHAT_TOOLS_H
#define CHAT_TOOLS_H
#include <stdint.h>
#include <stdio.h>


void set_tx(FILE *fileptr);

/*
 * read in a file into memory:
 * on success returns a pointer to a allocated sting
 * (has to be passed to free).
 * on error NULL is returned and errno is set.
 */
char *read_in_file(char *path);

/*
 * expects the sessionid as a string (exactly 20 chars)
 * returns the user to the sessionID
 * or NULL if there was no match or on failure
 */
char *validate_session_id(char *session_id);

/*
 * sets a file append only
 */
int set_file_append_only(char *filename);

/*
 * get all information from a uname in users.db
 */
char *get_user_entry(char *uname);

/*
 * send a message to the clent (tx)
 * use like fprintf but whith flush...
 */
int send_msg(char *msg, ...);


/*
 * check if a sessionID is valid and respons the client
 * with an error message if the SessionID is not valid.
 */
int check_sessionID (char *sessionID);


void send_internal_server_error();

/*
 * is_patrt_of (const char *chatname, const char *uname)
 * checks if the uname is part of the chatname
 * returns 1 if true 0 for false and -1 on error
 */
int is_part_of(const char *chatname, const char *uname);


#endif // CHAT_TOOLS_H
