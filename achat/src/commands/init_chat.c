#include "chat_tools.h"
#include "commands.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int check_file(char *path) {
    FILE *tmp = fopen(path, "a");
    if (tmp == NULL)
        return -1;
    if (fclose(tmp) == EOF)
        return -1;
    return set_file_append_only(path);
}

static int check_dir(char *path) {

    struct stat st;
    if (stat(path, &st) == -1) {
        if (errno != ENOENT) {
            return -1;
        }
        if (mkdir(path, 0777) == -1) {
            return -1;
        }
    }
    return set_file_append_only(path);
}

int init_chat() {
    umask(0);
    // cheack and create dirs
    if (check_dir(ROOT_PATH) == -1)
        return -1;
    chmod(ROOT_PATH, 0777);
    if (check_dir(CHAT_PATH) == -1)
        return -1;
    // check if all default files are accessable
    // and if not create them...
    if (check_file(ROOT_PATH "active_sessions") == -1)
        return -1;
    if (check_file(ROOT_PATH "usedSessionIDs") == -1)
        return -1;
    if (check_file(ROOT_PATH "users.db") == -1)
        return -1;

    return 0;
}
