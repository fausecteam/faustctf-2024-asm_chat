#include "connection_handler.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 1337

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    // SIGIGNORE for SIGCHILD
    struct sigaction act = {
        .sa_handler = SIG_IGN,
        .sa_flags = SA_NOCLDWAIT,
    };
    struct sigaction oact;
    if (sigaction(SIGCHLD, &act, &oact) < 0) {
        die("sigaction");
    }


    // init conection_handler
    if (init_conection_handler() == -1) {
        die("init_connection_handler");
    }
    // create a socket for listening to incomming connections
    int serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
    if (serverSocket < 0)
        die("socket");

#ifdef DEBUG
    int flag = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
        die("setsockopt");
#endif

    // bind
    struct sockaddr_in6 name = {
        .sin6_family = AF_INET6,
        .sin6_port = htons(PORT),
        .sin6_addr = in6addr_any,
    };

    if (bind(serverSocket, (struct sockaddr *)&name, sizeof(name)) < 0)
        die("bind");

    // listen
    if (listen(serverSocket, SOMAXCONN) < 0)
        die("listen");

    // for loop for accept
    for (;;) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            perror("accept");
            continue;
        }
        handle_connection(serverSocket, clientSocket);
    }
}
