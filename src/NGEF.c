#include <stdio.h>
#include <unistd.h>
#include "partiton.c"

void main()
{
	NGEF_init("FS.txt", 4096);                //Creating partiton of 1GB = 1073741824 bytes
}
