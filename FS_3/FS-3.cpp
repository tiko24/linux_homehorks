#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

void verify(int ret, const char* msg) {
    if (ret < 0) {
        perror(msg);
        exit(errno);
    }
}

void shred(int descriptor, size_t len) {
    char *buffer = (char*)malloc(len);
    if (!buffer) {
        perror("malloc");
        exit(1);
    }

    for (size_t idx = 0; idx < len; ++idx)
        buffer[idx] = '0';

    lseek(descriptor, 0, SEEK_SET);
    write(descriptor, buffer, len);

    free(buffer);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    char *path = argv[1];
    int filedesc = open(path, O_RDWR);
    verify(filedesc, "open");

    off_t file_size = lseek(filedesc, 0, SEEK_END);
    verify(file_size, "lseek");

    printf("Shredding %s (%ld bytes)\n", path, file_size);

    for (int pass = 0; pass < 3; ++pass)
        shred(filedesc, file_size);

    close(filedesc);
    unlink(path);

    printf("File shredded and deleted.\n");
    return 0;
}

