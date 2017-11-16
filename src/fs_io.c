#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "io.h"
#include "fs.h"

uint32_t scanFile_cluster(uint32_t clusterNumber, char * dirname)
{
	uint8_t buff[CLUSTERSIZE];
	uint8_t str[32];
	uint8_t null;
	int counter = 0, cnt;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	mainBootRegion MainBootRegion = get_mainBootRegion();

	read_cluster(buff, clusterNumber);

	FileNameEntry.fileName[0] = 'A';

	for (int i = 0; i < 30; ++i)
	{
		if (FileNameEntry.fileName[i] == 0)
			null++;
	}

	if (clusterNumber == 5)
		counter = 96;

	while(counter <= CLUSTERSIZE && null != 30 )
	{
		cnt =0;
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
			return StreamExtensionEntry.firstCluster;

		counter += 96;

		for (int i = 0; i < 30; ++i)
		{
			if (FileNameEntry.fileName[i] == 0)
				null++;
		}
	}

	return 0;
}


uint32_t clusterof_path(char *path)									//Will return cluster of the parent dir in which file or folder is to be created
{
	int i, j ;
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

bool create_file(char * path, char * fileName, char * data)
{
	uint32_t freeCluster;
	uint32_t parentCluster;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	uint8_t str[CLUSTERSIZE];
	uint8_t buff[CLUSTERSIZE];
	uint8_t null = 1;
	uint32_t counter = 0;

	freeCluster = get_freeCluster();
	parentCluster = clusterof_path(path);

	read_cluster(buff, parentCluster);

	while(counter <= CLUSTERSIZE && null != 30)
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

		counter += 96;

		for (int i = 0; i < 30; ++i)
		{
			if (FileNameEntry.fileName[i] == 0)
				null++;
		}
	}

	counter -= 96;

	FileDirectoryEntry.EntryType = 0x85;
	FileDirectoryEntry.fileAttribute = 0x0010;

	StreamExtensionEntry.EntryType = 0xC0;
	StreamExtensionEntry.generalSecondaryFlags = 0x01;
	StreamExtensionEntry.nameLength = 0x1E;
	StreamExtensionEntry.firstCluster = freeCluster;

	FileNameEntry.EntryType = 0xC1;
	FileNameEntry.generalSecondaryFlags = 0x01;

	cti_fileDirectoryEntry(FileDirectoryEntry, buff+counter);
	cti_streamExtensionEntry(StreamExtensionEntry, buff+counter+32);
	cti_fileNameEntry(FileNameEntry, buff+counter+64);

	write_cluster(buff, parentCluster);

	for (int i = 0; i < 30; ++i)
		FileNameEntry.fileName[i] = fileName[i];

	if(write_cluster(data, freeCluster))
		return true;

	else
		return false;
}

uint8_t * read_file(char * path, char * buff, char * data)
{
	uint32_t freeCluster;
	uint32_t parentCluster;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	uint8_t str[CLUSTERSIZE];
	uint8_t buff1[CLUSTERSIZE];
	uint8_t null = 1;
	uint32_t counter = 0;

	parentCluster = clusterof_path(path);

	read_cluster(buff, parentCluster);

	while(counter <= CLUSTERSIZE && null != 30)
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

		counter += 96;

		for (int i = 0; i < 30; ++i)
		{
			if (FileNameEntry.fileName[i] == 0)
				null++;
		}
	}

	counter -= 96;

	
	if(read_cluster(data, freeCluster))
		return data;

	else
		return NULL;
}