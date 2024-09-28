#include "chat_tools.h"
#include "commands.h"
#include <stdio.h>

int help() {
    // print commands and there usage
    char *help_str = "List of commands:\n"
                     "login/out:\n"
                     "register <username> <password>\n"
                     "login <username> <password>\n\n"
                     "create new chats:\n"
                     "start-chat <SessionID> <target_user>\n\n"
                     "send/recv msgs:\n"
                     "send <SessionID> <chat_name> <message>\n"
                     "read <SessionID> <chat_name>\n\n"
                     "search chats:\n"
                     "list <SessionID>\n"
                     "list-users <SessionID>\n" // maybe chage to no args
                     "search <SessionID> <Query>\n\n"
                     "misc:\n"
                     "help\n"
                     "emojis\n";
    return send_msg(help_str);
}
int emojis() {
    // print some fun ascii-art emojis
    char *emojis = "(╯°□°)╯\n"
                   "<(^_^)>\n"
                   "¯\\(°_o)/¯\n"
                   "(⌐■_■)\n"
                   "(｡◕‿‿◕｡)\n"
                   "(-_-)\n"
                   "⊂(◉‿◉)つ\n"
                   "(ㆆ _ ㆆ)\n"
                   "(⌐⊙_⊙)\n"
                   "=^_^=\n";

    return send_msg(emojis);
}

int invalid_command(char *command) {
    // responde with invalild command etc...
    char *msg = "%s is an invalid command!\n"
                "Try 'help' for information about valid commands.\n";
    return send_msg(msg, command);
}
