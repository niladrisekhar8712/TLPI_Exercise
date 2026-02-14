#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_ALLOCS 100000
int main(int argc, char *argv[])
{
    ;
    printf("The initial program break: %p\n", sbrk(0));
    char *ptr[MAX_ALLOCS];
    int freeStep = 1;
    int freeMin = 0;
    int freeMax = MAX_ALLOCS - 1;
    int blockSize = 1000;

    for (int i = 0; i < MAX_ALLOCS; i++)
    {
        if ((ptr[i] = malloc(blockSize)) == NULL)
        {
            exit("malloc");
        };
    }

    printf("The current program break: %p\n", sbrk(0));

    for (int j = freeMin; j <= freeMax; j += freeStep)
    {
        free(ptr[j]);
    }
    printf("The current program break: %p\n", sbrk(0));
}