#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "partition.c"
// #include "io.c"
#include "fs_io.c"

int main()
{
	int choice;
	uint8_t buff1[CLUSTERSIZE];
	char data[CLUSTERSIZE];
	uint32_t clusterNumber;
	char filename[30];
	
	NGEF_init("FS", 7340032);                //Creating partiton of 1GB = 1073741824 bytes

	while(1)
	{
		printf("\n\n\t\t\t1. Write to a cluster\n");
		printf("\t\t\t2. Read a cluster\n");
		printf("\t\t\t3. Create a file\n");
		printf("\t\t\t4. Read contents of a file\n");
		printf("\t\t\t5. Help\n");
		printf("Enter your operation code : ");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:{
				printf("Enter cluster number to write : ");
				scanf("%" SCNu32 "", &clusterNumber);

				printf("Enter data : ");
				scanf("%s", data);


				if(write_cluster(data, clusterNumber))
					printf("Written !\n");
				else
					printf("Writing to cluster failed !\n");
			}
				break;

			case 2:{
				printf("Enter cluster number to read : \n");
				scanf("%" SCNu32 "", &clusterNumber);

				

				if(read_cluster(buff1, clusterNumber))
					printf("%s\n", buff1);
				else
					printf("Eror reading cluster %" PRIu32 "\n", clusterNumber);
			}
				break;

			case 3:{
				printf("Enter file name : ");
				scanf("%s", filename);
				fflush(stdin);
				printf("Enter file data : ");
				scanf("%s", data);
				
				if(create_file(filename, data))
				 	printf("Created File %s in root.\n\n", filename);

				else
					printf("Error creating file.\n");
			}
				break;

			case 4:{
				fflush(stdin);

				printf("Enter filename to read : ");
				// scanf("%s", filename);
				scanf("%s", filename);

				if(read_file(filename, buff1))
					printf("\n%s", buff1);
			}
				break;

			case 5:{
				printf("1. Write to a cluster\n");
				printf("Write to a particular cluster by passing \ndata string and a 32-bit cluster number.");
				printf("\n\n2. Read a cluster\n");
				printf("Read the data stored inside a particular cluster \nby passing string pointer where data will be returned \nand 32-bit cluster number.");
				printf("\n\n3. Create a file\n");
				printf("Create a file by passing its name(less than 30 characters) and data.\n");
				printf("\n\n4. Read contents of a file\n");
				printf("Read the data stored in a file by passing \nstring pointer where data will be returned \nand filename.\n");
			}
			break;

			case 0:
				return 0;
		}
	}
}