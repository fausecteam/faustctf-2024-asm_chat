#include "chat_tools.h"
#include "commands.h"
#include <alloca.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int create_chat(char *args) {
    char *sessionID = strtok(args, "\t ");
    char *target_user = strtok(NULL, " \t");
    if (sessionID == NULL) {
        send_msg("Usage: start-chat <SessionID> <target_user>\n");
        return -1;
    }
    // check sessionID and get uname
    char *uname = validate_session_id(sessionID);
    if (uname == NULL) {
        send_msg("Invalid SessionID: %s\n", sessionID);
        return -1;
    }
    // create_chat name
    // (lex. smaler name infront of the other one)
    size_t chat_name_len = strlen(uname) + 1 + strlen(target_user);
    char *chat_name = alloca(chat_name_len + 1);
    if (strcmp(uname, target_user) < 0) {
        snprintf(chat_name, chat_name_len + 1, "%s&%s", uname, target_user);
    } else {
        snprintf(chat_name, chat_name_len + 1, "%s&%s", target_user, uname);
    }
    // check for uname

    // check if target_user exists (better late than never lol)
    char *target_entry = get_user_entry(target_user);
    if (target_entry == NULL) {
        send_msg("An user with the name '%s' does not exist!\n", target_user);
        free(uname);
        return -1;
    }

    // create chat file name
    char *chat_file = alloca(strlen(CHAT_PATH) + strlen(chat_name) + 1);
    snprintf(chat_file, strlen(CHAT_PATH) + strlen(chat_name) + 1, "%s%s", CHAT_PATH, chat_name);
    // create chat_file

    FILE *chat = fopen(chat_file, "a");
    if (chat == NULL) {
        send_internal_server_error();
        free(uname);
        return -1;
    }
    if (fclose(chat) == EOF) {
        send_internal_server_error();
        free(uname);
        return -1;
    }

    // set chat append only
    int child = fork();
    if (child < 0)
        return -1;
    if (child == 0) {
        // child code
        char *argv[3];
        argv[0] = "./create_chat";
        argv[1] = chat_file;
        argv[2] = NULL;
        execv(argv[0], argv);
    }

    return send_msg("Created the chat: '%s'.\n", chat_name);
}
