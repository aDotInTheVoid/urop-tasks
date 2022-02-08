#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ALLOC_SIZE 1000000
#define N_ALLOCS UINT8_MAX

void main()
{
	uint8_t *ptrs[N_ALLOCS];
	for (uint8_t i = 0; i < N_ALLOCS; i++)
	{
		ptrs[i] = malloc(ALLOC_SIZE);
		memset(ptrs[i], i, ALLOC_SIZE);
	}
	for (uint8_t i = 0; i < N_ALLOCS; i++)
	{
		if (*ptrs[i] != i)
			printf("Wrong value for %d\n", i);
		free(ptrs[i]);
	}
}
