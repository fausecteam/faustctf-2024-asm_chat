#include "chat_tools.h"
#include "commands.h"

#include <alloca.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int list(char *args) {
    // parse args
    char *sessionID = strtok(args, " \t\n");
    if (sessionID == NULL) {
        send_msg("Usage: list <SessionID>\n");
    }
    char *uname = validate_session_id(sessionID);
    if (uname == NULL) {
        send_msg("Invalid SessionID: %s\n", sessionID);
        return -1;
    }
    send_msg("Chats:\n");
    // list dir etc...
    struct dirent **namelist;
    int n;

    // scandir returns the number of directory entries in the specified directory
    n = scandir(CHAT_PATH, &namelist, NULL, alphasort);
    if (n < 0) {
        send_internal_server_error();
        return -1;
    }
    for (int i = 0; i < n; i++) {
        if (namelist[i]->d_type == 0x8) {
            // check if uname is patrt of the chat...
            switch (is_part_of(namelist[i]->d_name, uname)) {
                case 0:
                    break;
                case 1:
                    send_msg("\t> %s\n", namelist[i]->d_name);
                    break;
                default:
                    send_internal_server_error();
            }
        }
        free(namelist[i]);
    }
    free(namelist);
    free(uname);
    return 0;
}

int list_users(char *args) {
    // parse args
    char *sessionID = strtok(args, " \t\n");
    if (sessionID == NULL) {
        send_msg("Usage: list-users <SessionID>\n");
    }
    // validate sessionID
    if (check_sessionID(sessionID) == -1)
        return -1;
    // read in all users.db and print only the names
    char *users = read_in_file(USER_DB);
    if (users == NULL)
        return -1;
    char *curr_user = NULL;
    char *tmp = users;
    send_msg("Users:\n");
    do {
        curr_user = strtok(tmp, "\t");
        if (curr_user == NULL) {
            break;
        }
        send_msg("\t> %s\n", curr_user);
        tmp = NULL;
    } while (strtok(NULL, "\n") != NULL);
    free(users);
    return 0;
}

static int search_in_chat(const char *chatname, const char *needle, char *curr_uname) {
    // create full chantname
    char *chatpath = alloca(strlen(CHAT_PATH) + strlen(chatname) + 1);
    strcpy(chatpath, CHAT_PATH);
    strcat(chatpath, chatname);
    char *chat = read_in_file(chatpath);
    if (chat == NULL)
        return -1;

    // iterate over the messages
    char *FORMAT = alloca(0x100);
    strcpy(FORMAT, "Searched as %s:\nIn '%s': %s\n\0");
    char *tmp = chat;
    // Messages can only be 0x100 huge because of recv_line limit...
    char *message = alloca(0x100 + 1);
    for (;;) {
        char *header1 = strtok(tmp, "\n");
        if (header1 == NULL) {
            break;
        }
        tmp = NULL;
        char *header2 = strtok(NULL, "\n");
        if (header2 == NULL) {
            break;
        }

        char *msg = strtok(NULL, "\n");
        if (msg == NULL) {
            break;
        }

        strcpy(message, header1);
        message[strlen(message)+1] = '\0';
        message[strlen(message)] = '\n';
        strcat(message, header2);
        message[strlen(message)+1] = '\0';
        message[strlen(message)] = '\n';
        strcat(message, msg);
        message[strlen(message)+1] = '\0';
        message[strlen(message)] = '\n';

        // if the message contains the needle
        // --> send the message to the user
        if (strstr(msg, needle) != NULL) {
            send_msg(FORMAT, curr_uname, chatname, message);
        }
    }
    free(chat);
    return 0;
}

int search(char *args) {
    // parse args
    char *sessionID = strtok(args, " \t\n");

    char *needle = strtok(NULL, " \t\n");
    if (sessionID == NULL || needle == NULL) {
        send_msg("Usage: search <SessionID> <search string>\n");
        return -1;
    }

    char *uname = validate_session_id(sessionID);
    if (uname == NULL) {
        send_msg("Invalid SessionID: %s\n", sessionID);
        return -1;
    }
    // get list of chats from the user
    // list dir etc...
    struct dirent **namelist;
    int n;

    // scandir returns the number of directory entries in the specified directory
    n = scandir(CHAT_PATH, &namelist, NULL, alphasort);
    if (n < 0) {
        send_internal_server_error();
        free(uname);
        return -1;
    }
    // iterate over the chats
    for (int i = 0; i < n; i++) {
        if (namelist[i]->d_type == 0x8) {
            // check if uname is patrt of the chat...
            switch (is_part_of(namelist[i]->d_name, uname)) {
                case 0:
                    break;
                case 1:
                    if (search_in_chat(namelist[i]->d_name, needle, uname) == -1)
                        send_internal_server_error();

                    break;
                default:
                    send_internal_server_error();
            }
        }
        free(namelist[i]);
    }
    free(namelist);
    free(uname);
    return 0;
}
