#ifndef HANDLE_COMMANDS_H
#define HANDLE_COMMANDS_H
#include <stdio.h>



/* init the command handler
 * (check or create the file structure etc.)
 * @returns 0 on success -1 if there was an error
 */
int init_command_handler();

/* Handle a singele recived command
 * @returns
 * 0 on success, 
 * 1 if the user ended the session and
 * -1 if an occured error.
 */
int handle_command(FILE *rx, FILE *tx);

#endif // HANDLE_COMMANDS_H
