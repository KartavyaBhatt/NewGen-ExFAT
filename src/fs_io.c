#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "io.c"
#include "partition.h"
#include "fs.h"
#include "conversion.c"


bool create_file(char * path, char * fileName)
{
	uint32_t freeCluster;
	uint32_t parentCluster;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	uint8_t str[CLUSTERSIZE];

	freeCluster = get_freeCluster();
	parentCluster = clusterof_path(path);
}


uint32_t clusterof_path(char *path)
{
	int i;
	char name[30];
	uint32_t parentClusterNumber = 5;

	for (i = 0;path[i] != '\0'; i++)
	{
		for (j = 0; path[i] != '/' || path[i] != '\0'; )
		{
			name[j] = path[i];
			i++;
			j++;
		}

		name[j] = '\0';

		parentClusterNumber = scanFile_cluster(parentClusterNumber, name);

		if (parentClusterNumber == 0)
		{
			return 0;
		}
	}

	return parentClusterNumber;
}

uint32_t scanFile_cluster(uint32_t clusterNumber, char * dirname)
{
	uint8_t buff[CLUSTERSIZE];
	uint8_t str[32];
	int counter = 0;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	mainBootRegion MainBootRegion = get_mainBootRegion();

	read_cluster(buff, clusterNumber);

	if (clusterNumber == 5)
		counter = 96;

	while(counter <= CLUSTERSIZE)
	{
		for (int i = 0+counter; i < 32; ++i)
		str[i] = buff[i];

		FileDirectoryEntry = cts_fileDirectoryEntry(str);

		for (int i = 32+counter; i < 64; ++i)
			str[i-32] = buff[i];

		StreamExtensionEntry = cts_streamExtensionEntry(str);

		for (int i = 64+counter; i < 96; ++i)
			str[i-64] = buff[i];

		FileNameEntry = cts_fileNameEntry(str);	

		for (int i = 0; i < 30; ++i)
		{
			if (FileNameEntry.fileName[i] == dirname[i])
			{
				cnt++;
			}
		}

		if (cnt == 30)
		{
			return StreamExtensionEntry.firstCluster;
		}

		counter += 96;
	}

	return 0;
}