#include "chat_tools.h"
#include "commands.h"
#include <alloca.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <unistd.h>

static uint64_t rand_uint64(void) {
    uint64_t rand_num;
    if (getrandom(&rand_num, sizeof(uint64_t), 0) != sizeof(uint64_t))
        return 0;
    return rand_num;
}

/*
 * get a uname and returns for that uname a new sessionID
 */
static char *create_new_session(char *uname) {
    char *sessionID = calloc(21, sizeof(char));
    if (sessionID == NULL)
        return NULL;
    int is_valid;
    do {
        is_valid = 1;
        uint64_t rand_num = rand_uint64();
        snprintf(sessionID, 21, "%.20lu", rand_num);

        // check active sessions
        char *tmp = validate_session_id(sessionID);
        if (tmp != NULL)
            is_valid = 0;
        free(tmp);
    } while (is_valid == 0);
    // add sessionID to active sessions file
    FILE *session_file = fopen(ACTIVE_SESSIONS_FILE, "a");
    if (session_file == NULL) {
        free(sessionID);
        return NULL;
    }
    char *entry = alloca(21 + strlen(uname) + 2);
    snprintf(entry, 21 + strlen(uname) + 2, "%s\t%s\n", sessionID, uname);

    if (write(fileno(session_file), entry, strlen(entry)) == -1) {
        free(sessionID);
        fclose(session_file);
        return NULL;
    }
    if (fclose(session_file) == EOF) {
        free(sessionID);
        return NULL;
    }
    return sessionID;
}

int login(char *args) {
    // pars arguments
    char *uname = strtok(args, " \t");
    char *passwd = strtok(NULL, "");
    if (uname == NULL || passwd == NULL) {
        send_msg("Usage: login <username> <password>\n");
        return -1;
    }

    char *user_entry = get_user_entry(uname);
    // check if user exists:
    if (user_entry == NULL) {
        send_msg("Invalid username!\n");
        return -1;
    }
    char *real_passwd = strtok(user_entry, "\t");
    if (strcmp(real_passwd, passwd) != 0) {
        send_msg("INVALID PASSWORD!\n");
        return -1;
    }
    char *new_session = create_new_session(uname);
    if (new_session == NULL) {
        send_msg("An error occured while creating the sessionID!\n");
        return -1;
    }
    send_msg("SessionID: %s\n", new_session);
    free(new_session);
    return 0;
}

int register_user(char *args) {
    // pars arguments
    char *uname = strtok(args, " \t");
    char *passwd = strtok(NULL, "\t\n");
    if (uname == NULL || passwd == NULL) {
        send_msg("Usage: register <username> <password>\n");
        return -1;
    }
    // check if username is valid (shall not contain '&' or numbers)
    for (int i = 0; i < strlen(uname); i++) {
        if (isalpha(uname[i]) == 0) {
            send_msg("Invalid username (%s)\nUsernames shall not contain numbers or '&'!\n");
            return -1;
        }
    }
    // check if username is already used
    char *tmp = get_user_entry(uname);
    if (tmp != NULL) {
        // username already exists
        free(tmp);
        send_msg("The username '%s' already exists\n", uname);
        return -1;
    }
    // create account (append to users.db)
    FILE *users_file = fopen(USER_DB, "a");
    if (users_file == NULL) {
        send_msg("An error occured while creating the user account!\n", args);
        return -1;
    }
    // create new entry
    char *new_entry = calloc(strlen(uname) + 1 + strlen(passwd) + 1 + 1, sizeof(char));
    if (new_entry == NULL) {
        send_msg("An error occured while creating the user account!\n", args);
        return -1;
    }

    strcpy(new_entry, uname);
    new_entry[strlen(uname)] = '\t';
    strcat(new_entry, passwd);
    new_entry[strlen(new_entry)] = '\n';
    if (write(fileno(users_file), new_entry, strlen(new_entry)) == -1) {
        send_msg("An error occured while creating the user account!\n", args);
        free(new_entry);
        return -1;
    }
    free(new_entry);
    if (fclose(users_file) == EOF)
        return -1;
    char *new_session = create_new_session(uname);
    if (new_session == NULL) {
        send_msg("An error occured while creating the sessionID!\n");
        return -1;
    }
    send_msg("SessionID: %s\n", new_session);
    free(new_session);
    return 0;
}
