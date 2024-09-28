#include "chat_tools.h"
#include "commands.h"
#include <errno.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static FILE *tx;

void set_tx(FILE *fileptr) {
    tx = fileptr;
}

// musl defines the second argument as int only, which breaks constants
int ioctl(int fd, unsigned int op, ...);

char *read_in_file(char *path) {
    struct stat st;
    if (stat(path, &st) == -1)
        return NULL;
    char *contents = malloc((size_t)(st.st_size + 1));
    if (contents == NULL)
        return NULL;

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        free(contents);
        return NULL;
    }
    if (read(fileno(file), contents, (size_t)st.st_size) == -1) {
        int tmp = errno;
        fclose(file);
        free(contents);
        errno = tmp;
        return NULL;
    }
    contents[(size_t)st.st_size] = '\0';
    if (fclose(file) == EOF) {
        free(contents);
        return NULL;
    }
    return contents;
}

char *validate_session_id(char *session_id) {
    if (strlen(session_id) != 20)
        return NULL;
    char *sessions = read_in_file(ACTIVE_SESSIONS_FILE);
    if (sessions == NULL)
        return NULL;
    char *found_entry = strstr(sessions, session_id);
    if (found_entry == NULL) {
        free(sessions);
        return NULL;
    }
    for (; *found_entry != '\t'; found_entry++) {
    }
    found_entry++;
    char *tmp = strtok(found_entry, "\n");
    if (tmp == NULL) {
        free(sessions);
        return NULL;
    }
    char *retval = strdup(tmp); // error will be handeld by caller
    free(sessions);
    return retval;
}

int set_file_append_only(char *filename) {
    int fd;
    int flags;

    // Open the file
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return -1;
    }

    // Get current flags
    if (ioctl(fd, FS_IOC_GETFLAGS, &flags) == -1) {
        close(fd);
        return -1;
    }

    // Set the append-only flag
    flags |= FS_APPEND_FL;

    // Set the new flags
    if (ioctl(fd, FS_IOC_SETFLAGS, &flags) == -1) {
        close(fd);
        return -1;
    }

    // Close the file
    close(fd);
    return 0;
}

char *get_user_entry(char *uname) {
    errno = 0;
    char *users_db = read_in_file(USER_DB);
    if (users_db == NULL)
        return NULL;
    char *curr_uname = NULL;
    char *curr_info = NULL;
    char *tmp = users_db;
    do {
        curr_uname = strtok(tmp, "\t");
        if (curr_uname == NULL)
            break;

        tmp = NULL;
        curr_info = strtok(tmp, "\n");
        if (curr_info == NULL)
            break;
        if (strcmp(curr_uname, uname) == 0) {
            char *ret_val = strdup(curr_info);
            free(users_db);
            return ret_val;
        }
    } while (1);
    free(users_db);
    return NULL;
}

int send_msg(char *msg, ...) {
    va_list args;
    va_start(args, msg);
    if (vfprintf(tx, msg, args) < 0)
        return -1;
    if (fflush(tx) == EOF)
        return -1;
    return 0;
}

int check_sessionID(char *sessionID) {
    char *tmp = validate_session_id(sessionID);
    if (tmp == NULL) {
        // invalid session_id
        send_msg("Invalid SessionID: %s\n", sessionID);
        return -1;
    }
    free(tmp);
    return 0;
}

void send_internal_server_error() { send_msg("Internal Server error! :(\n"); }


int is_part_of(const char *chatname, const char *uname) {
    char *tmp_chatname = strdup(chatname);
    if (tmp_chatname == NULL)
        return -1;

    char *uname1 = strtok(tmp_chatname, "&");
    if (uname1 == 0) {
        free(tmp_chatname);
        return -1;
    }
    char *uname2 = strtok(NULL, "");
    if (uname2 == 0) {
        free(tmp_chatname);
        return -1;
    }

    if(strcmp(uname, uname1) == 0) {
        free(tmp_chatname);
        return 1;
    }
    if(strcmp(uname, uname2) == 0) {
        free(tmp_chatname);
        return 1;
    }
    free(tmp_chatname);
    return 0;
}
