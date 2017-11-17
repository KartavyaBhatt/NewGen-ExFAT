#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "partition.c"
// #include "io.c"
#include "fs_io.c"

void main()
{
	uint8_t buff1[CLUSTERSIZE];
	uint8_t buff[32];
	uint8_t str[32];
	char data[CLUSTERSIZE];
	uint32_t clusterNumber;
	
	NGEF_init("FS", 7340032);                //Creating partiton of 1GB = 1073741824 bytes

	


	// if(write_cluster(data, 5))
	// {
	// 	read_cluster(buff1, 5);

	// 	if (buff1)
	// 	{
	// 		printf("%s\n", buff1);
	// 	}
	// }

	char *path = "/";
	char filename[30] = {'a', 'b', 'c'};
	
	scanf("%s", data);

	// uint64_t pl = strlen(path);
	// uint64_t dl = strlen(data);

	// printf("%" PRIu64 "\n", pl);
	// printf("%" PRIu64 "\n", dl);
	
	printf("Data in Main : %s\n", data);

	if(create_file(filename, data))
	 	printf("Created FIle in root.\n");

	 read_file(filename, buff1);
	 printf("Data : \n%s", buff1);
}