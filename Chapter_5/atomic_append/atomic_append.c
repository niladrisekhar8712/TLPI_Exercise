#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

void errExit(char *msg)
{
    printf("%s", msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (!(argc == 3 || argc == 4) || strcmp(argv[1], "--help") == 0)
    {
        errExit("Usage: <atomic_append> filename num-bytes [x]\n");
    }
    int fileFd;
    int flags = O_CREAT | O_WRONLY | O_TRUNC;
    int filePerm = S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH;
    int num_bytes = atoi(argv[2]);
    printf("Bytes: %d\n", num_bytes);
    if (argc == 3)
    {
        printf("[PID: %d] : O_APPEND is applied\n", getpid());
        flags |= O_APPEND;
    }
    else
    {
        printf("[PID: %d] : O_APPEND is not applied, lseek to end of the file everytime \n", getpid());
    }
    fileFd = open(argv[1], flags, filePerm);
    if (fileFd == -1)
    {
        errExit("Error in opening file\n");
    }
    char buf[1];
    int i = 0;
    while (i++ < num_bytes)
    {
        // printf("hi\n");
        buf[0] = rand() % 26 + 'a';
        if (argc == 4 && strcmp(argv[3], "x") == 0 && (lseek(fileFd, 0, SEEK_END) == -1))
        {
            errExit("Error in lseek or Usage: <atomic_append> filename num-bytes [x]\n");
        }
        if (write(fileFd, buf, 1) == -1)
        {
            errExit("Error in write\n");
        }
    }

    if (close(fileFd) == -1)
    {
        errExit("Error in closing file\n");
    }
    exit(EXIT_SUCCESS);
}