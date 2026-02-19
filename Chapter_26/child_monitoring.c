#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>

char *getTime()
{
    time_t currTime = time(NULL);
    char *timeStr = ctime(&currTime);
    timeStr[strlen(timeStr) - 1] = '\0';
    return timeStr;
}

int errExit(char *s)
{
    printf("%s", s);
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc == 1 || strcmp(argv[1], "--help") == 0)
    {
        errExit("Usage: <sleep_time_1> <sleep_time_2> ...\n");
    }

    int numChild = argc - 1;
    int childPid;
    for (int i = 1; i <= numChild; i++)
    {
        switch (childPid = fork())
        {
        case -1:
            errExit("Fork error\n");
        case 0:
            int sleepTime = abs(atoi(argv[i]));
            printf("[%s] Child [%d] commences for sleeping for %d seconds\n", getTime(), getpid(), sleepTime);
            sleep(sleepTime);
            // printf("[%s] Child [%d] done sleeping for %d seconds: \n", getTime(), getpid(), sleepTime);
            _exit(EXIT_SUCCESS);
        default:
        }
    }

    while (1)
    {
        childPid = wait(NULL);
        if (childPid == -1)
        {
            if (errno == ECHILD)
            {
                printf("No more Children\n");
                _exit(EXIT_SUCCESS);
            }
            else
            {
                errExit("Error on wait\n");
            }
        }
        else
        {
            printf("[%s] wait returned with pid = %d\n", getTime(), childPid);
        }
    }
    _exit(EXIT_SUCCESS);
}