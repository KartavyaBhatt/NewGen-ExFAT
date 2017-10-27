#include <stdio.h>
#include "utils.h"

static long long int SIZE;

static FILE *volume;

int NGEF_init(const char *partition_name, long long int size)
{
		volume = fopen(partition_name, "a+");

		if (volume == NULL)
			return 0;

		ftruncate(fileno(volume), size);
		
		SIZE = size;

		return 1;
}


