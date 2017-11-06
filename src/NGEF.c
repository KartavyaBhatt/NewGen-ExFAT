#include <stdio.h>
#include <unistd.h>
#include "partition.c"

void main()
{
	NGEF_init("FS.txt", 1073741824);                //Creating partiton of 1GB = 1073741824 bytes
	mount_NGFS();
}