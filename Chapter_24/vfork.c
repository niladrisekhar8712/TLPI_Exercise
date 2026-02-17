#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
/* demonstration that closing an FD by child process does not
affect the corresponding FD in the parent*/
int errExit(char *msg)
{
    printf("%s", msg);
    exit(EXIT_FAILURE);
}
int main(int argc, char *argv[])
{
    int filePrem = S_IRUSR | S_IWUSR;
    int fd = open("test1.txt", O_TRUNC | O_CREAT | O_RDWR, filePrem);

    int childpid;
    switch (childpid = vfork())
    {
    case -1:
        errExit("Child process creation failure\n");
    case 0:
        if (close(fd) == -1)
        {
            errExit("Fd closure failure\n");
        }

        printf("FD closed by child process\n");
        exit(EXIT_SUCCESS);
    default:
        break;
    }
    if (wait(NULL) == -1)
    {
        errExit("error in wait\n");
    }
    printf("Child has exited\n");
    int f;
    if ((f = write(fd, "abcd", 2)) != -1)
    {
        printf("Successfully written %d bytes to fd\n", f);
        if (close(fd) == -1)
        {
            errExit("There some error occured from write\n");
        }
        else
            printf("Confirmed that closing a fd opend by child is not affected the corresponding fd in parent\n");
    }
    else
    {
        errExit("There some error occured\n");
    }
    exit(EXIT_SUCCESS);
}