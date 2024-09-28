#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the file
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Set the file to append-only mode
    int flags = FS_APPEND_FL;
    if (ioctl(fd, FS_IOC_SETFLAGS, &flags) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("File %s set to append-only.\n", argv[1]);

    // Close the file
    close(fd);

    return 0;
}
