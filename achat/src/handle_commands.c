#include "handle_commands.h"
#include "./commands/commands.h"
#include "commands/chat_tools.h"
#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define EXPECTED_SIZE 0x100

int init_command_handler() { return init_chat(); }

static char *rcv_line(FILE *rx) {
    size_t alloced_size = EXPECTED_SIZE;
    char *line = malloc(alloced_size);
    if (line == NULL)
        return NULL;
    size_t cntr = 0;
    int tmp_char = 0;
    for (;;) {
        tmp_char = fgetc(rx);
        if (tmp_char == EOF) {
            // in case of EOF (closed socket) or other errors return NULL
            free(line);
            return NULL;
        }
        if (tmp_char == '\n') {
            line[cntr] = '\0';
            return line;
        }
        if (cntr + 1 == alloced_size) {
            alloced_size += EXPECTED_SIZE;
            char *tmp_alloc = realloc(line, alloced_size);
            if (tmp_alloc == NULL) {
                free(line);
                return NULL;
            }
            line = tmp_alloc;
        }
        line[cntr] = (char)tmp_char;
        cntr++;
    }
}

static int exec_command(char *command, char *args) {
    // convert all commands to only lowercase
    for (int i = 0; command[i]; i++) {
        command[i] = (char)tolower(command[i]);
    }

    // begin of huge  if ladder for commands... :(

    // login/out:
    if (strcmp(command, "login") == 0)
        return login(args);
    if (strcmp(command, "register") == 0)
        return register_user(args);

    // create new chats/leave chats
    if (strcmp(command, "start-chat") == 0)
        return create_chat(args);

    // send/recv msgs
    if (strcmp(command, "send") == 0)
        return send(args);
    if (strcmp(command, "read") == 0)
        return read_chat(args);

    // search chats
    if (strcmp(command, "list") == 0)
        return list(args);
    if (strcmp(command, "list-users") == 0)
        return list_users(args);
    if (strcmp(command, "search") == 0)
        return search(args);

    // misc
    if (strcmp(command, "help") == 0)
        return help();
    if (strcmp(command, "emojis") == 0)
        return emojis();
    // notice over wrong command
    return invalid_command(command);
}

int handle_command(FILE *rx, FILE *tx) {
    set_tx(tx);
    send_msg("$ ");
    // rcv line
    char *input = rcv_line(rx);
    if (input == NULL)
        return -1;

    // parse line
    char *command = strtok(input, " ");
    char *arguments = strtok(NULL, "");
#ifdef DEBUG
    printf("%s, %s\n", command, arguments);
#endif
    errno = 0;
    int retval = exec_command(command, arguments);
    if (errno == 0) {
        retval = 0;
    }
    free(input);
    return retval;
}
