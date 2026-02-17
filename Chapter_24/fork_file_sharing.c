#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int errExit(char *msg)
{
    printf("%s", msg);
    exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
    int parentPid = getpid();
    int fd, flags;
    fd = open("test.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        errExit("File opening error\n");
    }

    printf("File offset before fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));
    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        errExit("fcntl error\n");
    }
    printf("O_APPEND flag before fork(): %s\n", (flags & O_APPEND) ? "on" : "off");
    int childpid;
    switch (childpid = fork())
    {
    case -1:
        errExit("error in fork()\n");
    case 0:
        long long off_t = (long long)lseek(fd, 1000, SEEK_CUR);
        if (off_t == -1)
        {
            errExit("error in lssek()\n");
        }
        // printf("The offset after fork(): %lld\n", (long long)off_t);
        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags | O_APPEND) == -1)
        {
            errExit("error in fcntl\n");
        }
        // printf("O_APPEND flag after fork(): %s\n", (flags & O_APPEND) ? "on" : "off");

        exit(EXIT_SUCCESS);

    default:
        if (wait(NULL) == -1)
        {
            errExit("error in wait\n");
        }
        printf("Child has exited\n");
        printf("File offset after fork(): %lld\n", (long long)lseek(fd, 0, SEEK_CUR));
        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
        {
            errExit("fcntl error\n");
        }
        printf("O_APPEND flag after fork(): %s\n", (flags & O_APPEND) ? "on" : "off");
    }
    exit(EXIT_SUCCESS);
}