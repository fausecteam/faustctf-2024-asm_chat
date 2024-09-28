#define _GNU_SOURCE
#include "connection_handler.h"
#include "handle_commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <grp.h>
#include <sys/random.h>
#include <sys/resource.h>

static void die(const char *msg) {
    #ifdef DEBUG
    perror(msg);
    #endif
    exit(EXIT_FAILURE);
}

static void drop_privs() {
    uid_t uid;
    getrandom(&uid, sizeof(uid), 0);

    setgroups(0, NULL);
    setresuid(uid, uid, uid);
    setresgid(uid, uid, uid);

    struct rlimit proc = {.rlim_cur = 50, .rlim_max = 50};
    struct rlimit as = {.rlim_cur = 10 * 1024 * 1024, .rlim_max = 10 * 1024 * 1024};
    struct rlimit cpu = {.rlim_cur = 10, .rlim_max = 10};
    setrlimit(RLIMIT_NPROC, &proc);
    setrlimit(RLIMIT_AS, &as);
    setrlimit(RLIMIT_CPU, &cpu);
}

int init_conection_handler() { return init_command_handler(); }

int handle_connection(int serverSocket, int clientSocket) {
    pid_t child = fork();
    if (child == -1) {
        return -1;
    }
    if (child != 0) {
        // parent code
        return close(clientSocket);
    }
    // child code
    // inti rx and tx file descriptors
    if (close(serverSocket))
        die("close (child)");
    FILE *tx = fdopen(clientSocket, "w");
    if (tx == NULL)
        die("fdopen (child)");

    int tmp = dup(clientSocket);
    if (tmp == -1)
        die("dup (child)");
    FILE *rx = fdopen(tmp, "r");
    if (rx == NULL)
        die("fdopen (child)");

    drop_privs();

    for (;;) {
        switch (handle_command(rx, tx)) {
            case -1:
                die("handle_command (child)"); // no break needed because die does not return
            case 1:
                // close tx and rx
                if (fclose(rx) == EOF)
                    die("fclose (child)");
                if (fclose(tx) == EOF)
                    die("fclose (child)");
                // do not return...
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }
}
