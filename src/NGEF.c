#include <stdio.h>
#include <unistd.h>
#include "partition.c"

void main()
{
	NGEF_init("FS.txt", (8388608));                //Creating partiton of 1GB = 1073741824 bytes
	printf("Mounting NGEF\n");
	mount_NGEF();
}