#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

/*
 * Init ...
 * @returns 0 on success -1 if there was an error
 */
int init_conection_handler();

/*
 * Forks and handles the new connection.
 * @returns 0 on success -1 if there was an error
 * in the parrent process
 */
int handle_connection(int serverSocket, int clientSocket);

#endif // CONNECTION_HANDLER_H
