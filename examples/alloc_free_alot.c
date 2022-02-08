#include <string.h>
#include <stdlib.h>

#define N_ALLOCS 1000
#define ALLOC_SIZE 1000000000

int main()
{
    for (int i = 0; i < N_ALLOCS; i++)
    {
        void *ptr = malloc(ALLOC_SIZE);
        // Ensure linux isnt just giving us empty pages
        memset(ptr, 0, ALLOC_SIZE);
        free(ptr);
    }
}