#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "io.h"
#include "fs.h"
#include "partition.h"
/*
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

	while(counter <= CLUSTERSIZE && null != 30)
	{
		cnt = 0;
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


uint32_t clusterof_path(char *path, uint64_t nameLength)									//Will return cluster of the parent dir in which file or folder is to be created
{
	int i, j ;
	char name[30];
	uint32_t parentClusterNumber = 5;
	uint64_t one = 0x0001;

	for (i = 0;path[i] != '\0'; i++)
	{
		for (j = 0; (path[i] != '/' || path[i] != '\0') && nameLength != 0; )
		{
			name[j] = path[i];
			i++;
			j++;
			printf("nameLength : %" PRIu64 "\n", nameLength);
			nameLength = nameLength - one;
		}

		name[j] = '\0';

		parentClusterNumber = scanFile_cluster(parentClusterNumber, name);

		printf("parentClusterNumber : %" PRIu32 "\n", parentClusterNumber);

		if (parentClusterNumber == 0)
		{
			return 0;
		}
	}

	return parentClusterNumber;
}

bool create_file(char * path, char * fileName, char * data, uint64_t pathLength, uint64_t length)
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
	parentCluster = clusterof_path(path, pathLength);

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
	for (int i = 0; i < 30; ++i)
		FileNameEntry.fileName[i] = fileName[i];

	cti_fileDirectoryEntry(FileDirectoryEntry, buff+counter);
	cti_streamExtensionEntry(StreamExtensionEntry, buff+counter+32);
	cti_fileNameEntry(FileNameEntry, buff+counter+64);

	write_cluster(buff, parentCluster);

	for (int i = 0; i < (int)(length/CLUSTERSIZE)+1; i+CLUSTERSIZE)
	{
		if(!write_cluster(data+(i*CLUSTERSIZE), freeCluster))
			return false;
	}
	

	return true;
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
*/

bool create_file(char *fileName, char *data)
{
	uint8_t buff[CLUSTERSIZE];
	uint8_t str[32];
	uint8_t null;
	uint32_t freeCluster, prev;
	int counter = 0, cnt;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	mainBootRegion MainBootRegion = get_mainBootRegion();

	freeCluster = get_freeCluster();

	read_cluster(buff, 5);

	counter = 96;

	while(counter <= CLUSTERSIZE)
	{
		cnt = 0;

		for (int i = 0+counter; i < 32; ++i)
		str[i] = buff[i];

		FileDirectoryEntry = cts_fileDirectoryEntry(str);

		if (FileDirectoryEntry.EntryType == 0)
			break;

		for (int i = 32+counter; i < 64; ++i)
			str[i-32 - counter] = buff[i];

		StreamExtensionEntry = cts_streamExtensionEntry(str);

		prev = StreamExtensionEntry.firstCluster;

		for (int i = 64+counter; i < 96; ++i)
			str[i-64 - counter] = buff[i];

		FileNameEntry = cts_fileNameEntry(str);

		counter += 96;
	}

	if (counter != 96)
		counter -= 96;
	
	// Debug :: printf("counter : %d\n", counter);

	FileDirectoryEntry.EntryType = 0x85;
	FileDirectoryEntry.fileAttribute = 0x0000;

	StreamExtensionEntry.EntryType = 0xC0;
	StreamExtensionEntry.generalSecondaryFlags = 0x01;
	StreamExtensionEntry.nameLength = 0x1E;
	StreamExtensionEntry.firstCluster = freeCluster;

	FileNameEntry.EntryType = 0xC1;
	FileNameEntry.generalSecondaryFlags = 0x01;
	for (int i = 0; i < 30; ++i)
		FileNameEntry.fileName[i] = fileName[i];

	cti_fileDirectoryEntry(FileDirectoryEntry, buff+counter);
	cti_streamExtensionEntry(StreamExtensionEntry, buff+counter+32);
	cti_fileNameEntry(FileNameEntry, buff+counter+64);

	// Debug :: FileDirectoryEntry = cts_fileDirectoryEntry(buff+counter);
	// Debug :: printf("EntryType : %" PRIu8 "\n", FileDirectoryEntry.EntryType);

	if(write_cluster(buff, 5) && write_cluster(data, freeCluster))
	{
		printf("written to cluster : %" PRIu32 "\n", freeCluster);
		set_cluster_bitmap1(freeCluster);
		// Debug :: read_cluster(data, 5);
		// Debug :: FileDirectoryEntry = cts_fileDirectoryEntry(data+counter);
		// Debug :: printf("EntryType : %" PRIu8 "\n", FileDirectoryEntry.EntryType);
		return true;
	}

	return 0;
}

bool read_file(char *fileName, char *str)
{
	uint8_t buff[CLUSTERSIZE];
	uint8_t null;
	uint32_t freeCluster, prev;
	int counter = 0, cnt;
	fileDirectoryEntry FileDirectoryEntry;
	streamExtensionEntry StreamExtensionEntry;
	fileNameEntry FileNameEntry;
	mainBootRegion MainBootRegion = get_mainBootRegion();

	freeCluster = get_freeCluster();

	read_cluster(buff, 5);

	counter = 96;

	while(counter <= CLUSTERSIZE)
	{
		cnt = 0;

		FileDirectoryEntry = cts_fileDirectoryEntry(buff+counter);

		if (FileDirectoryEntry.EntryType == 0)
		{
			printf("No such file/directory.\n");
			return false;
			break;
		}

		
		StreamExtensionEntry = cts_streamExtensionEntry(buff+counter+32);
		prev = StreamExtensionEntry.firstCluster;
		// Debug :: printf("firstCluster : %" PRIu32 "\n", StreamExtensionEntry.firstCluster);

		
		FileNameEntry = cts_fileNameEntry(buff+counter+64);

		for (int i = 0; i < 30; ++i)
		{
			if(FileNameEntry.fileName[i] == fileName[i])
				cnt++;
		}

		if (cnt == 30)
		{
			break;
		}

		counter += 96;
	}

	if(read_cluster(str, prev))
		return true;

	else
		return false;
}