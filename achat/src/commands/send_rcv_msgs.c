#include "chat_tools.h"
#include "commands.h"
#include <alloca.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int send(char *args) {
    // parse args
    char *sessionID = strtok(args, " \t\n");
    char *chat = strtok(NULL, " \t\n");
    char *message = strtok(NULL, "");
    if (sessionID == NULL || chat == NULL || message == NULL) {
        send_msg("Usage: send <SessionID> <chat_name> <message>\n");
        return -1;
    }
    // validate sessionID
    char *uname = validate_session_id(sessionID);
    if (uname == NULL) {
        send_msg("Invalid SessionID: %s\n", sessionID);
        return -1;
    }

    // check if user is allowed to access the chat
    if (is_part_of(chat, uname) != 1) {
        send_msg("Invalid chat (%s) for the user (%s)\n", chat, uname);
        return -1;
    }

    // create absolute path to the chat
    size_t chat_file_path_size = strlen(CHAT_PATH) + strlen(chat) + 1;
    char *chat_file_path = alloca(chat_file_path_size);
    snprintf(chat_file_path, chat_file_path_size, "%s%s", CHAT_PATH, chat);
    // check if chat exists
    struct stat st;
    if (stat(chat_file_path, &st) == -1) {
        if (errno == ENOENT) {
            send_msg("The chat '%s' does nont exist!\n(statr a new chat with 'start-chat')\n", chat);
            errno = 0;
            return -1;
        }
        free(uname);
        send_internal_server_error();
        return -1;
    }
    // create the message header
    // get time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char time_str[64];
    errno = 0;
    if (strftime(time_str, sizeof(time_str), "%c", tm) == 0) {
        if (errno != 0) {
            free(uname);
            send_internal_server_error();
            return -1;
        }
    }
    // craft real_message:
    const char *seperator_line = "-------------------------------------------------\n";
    size_t final_message_size = strlen(seperator_line);
    final_message_size += strlen(time_str) + 1;
    final_message_size += strlen(uname) + 1;
    final_message_size += strlen(message) + 2;
    char *final_message = alloca(final_message_size);
    snprintf(final_message, final_message_size, "%s%s\t%s\n%s\n", seperator_line, time_str, uname, message);
    // write the message to the file
    free(uname);
    uname = NULL;
    FILE *chat_file = fopen(chat_file_path, "a");
    if (chat_file == NULL) {
        send_internal_server_error();
        return -1;
    }
    if (write(fileno(chat_file), final_message, final_message_size - 1) == -1) {
        send_internal_server_error();
        return -1;
    }
    if (fclose(chat_file) == EOF) {
        send_internal_server_error();
        return -1;
    }
    send_msg("send the Message:\n%s", final_message);
    return 0;
}

int read_chat(char *args) {
    // parse args
    char *sessionID = strtok(args, " \t\n");
    char *chat = strtok(NULL, " \t\n");
    if (sessionID == NULL || chat == NULL) {
        send_msg("Usage: read <SessionID> <chat_name>");
        return -1;
    }
    // validate sessionID
    char *uname = validate_session_id(sessionID);
    if (uname == NULL) {
        send_msg("Invalid SessionID: %s\n", sessionID);
        return -1;
    }

    // check if user is allowed to access the chat
    if (is_part_of(chat, uname) != 1) {
        send_msg("Invalid chat (%s) for the user (%s)\n", chat, uname);
        return -1;
    }

    // create absolute path to the chat
    size_t chat_file_path_size = strlen(CHAT_PATH) + strlen(chat) + 1;
    char *chat_file_path = alloca(chat_file_path_size);
    snprintf(chat_file_path, chat_file_path_size, "%s%s", CHAT_PATH, chat);
    // check if chat exists
    struct stat st;
    if (stat(chat_file_path, &st) == -1) {
        if (errno == ENOENT) {
            send_msg("The chat '%s' does nont exist!\n(start a new chat with 'start-chat')\n", chat);
            errno = 0;
            return -1;
        }
        free(uname);
        send_internal_server_error();
        return -1;
    }
    free(uname);
    // sendfile contents to the client
    char *chat_file = read_in_file(chat_file_path);
    if (chat_file == NULL) {
        send_internal_server_error();
        return -1;
    }
    int retval = send_msg("Chat %s:\n%s\n", chat, chat_file);
    free(chat_file);
    return retval;
}
