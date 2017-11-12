#include <stdio.h>
#include <unistd.h>
#include "partition.c"
#include "io.c"

void main()
{
	char *buff1;
	char buff[CLUSTERSIZE];

	buff1 = "File";
	NGEF_init("FS", 7340032);                //Creating partiton of 1GB = 1073741824 bytes

	if(write_cluster(buff1, 3))
	{
		printf("Write Sucessful to cluster 3.\n");
		read_cluster(buff, 3);

		if (buff)
		{
			printf("Reading cluster 3 : %s\n", buff);
		}
	}
}