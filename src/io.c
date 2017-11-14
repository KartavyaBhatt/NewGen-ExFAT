#include <stdio.h>
#include <string.h>
#include "partition.c"
#include "fs.h"
#include "conversion.c"

bool write_cluster(char *data, long int clusterNumber)
{
	mainBootRegion MainBootRegion = get_mainBootRegion();
	
	if (clusterNumber <= MainBootRegion.clusterCount && data != NULL)
	{
		FILE *volume = get_volume();
		cluster Cluster;
		

		strcpy(Cluster.data,data);

		// Debug :: printf("Data : %s\n", data);
		// Debug :: printf("Cluster data : %s\n", Cluster.data);

		fseek(volume, (MainBootRegion.clusterHeapOffset*SECTOR_SIZE) + (clusterNumber*CLUSTERSIZE), 0);
		
		if(fwrite(&Cluster, sizeof(Cluster), 1, volume) == 1)
			return true;
		else
			return false;
	}

	else
		return false;
}

bool read_cluster(char *buff, long int clusterNumber)
{
	mainBootRegion MainBootRegion = get_mainBootRegion();
	
	if (clusterNumber <= MainBootRegion.clusterCount && buff != NULL)
	{		
		FILE *volume = get_volume();
		cluster Cluster;

		fseek(volume, (MainBootRegion.clusterHeapOffset*SECTOR_SIZE) + (clusterNumber*CLUSTERSIZE), 0);
		
		if(fread(&Cluster, sizeof(Cluster), 1, volume) == 1)
		{
			// Debug :: printf("Read Cluster Data : %s\n", Cluster.data);
			strcpy(buff, Cluster.data);
			// Debug :: printf("Read Buff Data : %s\n", buff);
			return true;
		}
		else
			return false;
	}
}

/*
uint32_t get_freeCluster()
{
	FILE *volume = get_volume();
	cluster Cluster;

	fseek(volume, (MainBootRegion.clusterHeapOffset*SECTOR_SIZE), 0);

	for (uint32_t i = 2; i < MainBootRegion.clusterCount; ++i)
	{
		if(fread(&Cluster, sizeof(Cluster), 1, volume) == 1)
		{
			if (Cluster.data == NULL)
			{
				return i;
			}
		}
	}
}
*/

static bool rootDir_init()
{
	FILE *volume = get_volume();
	mainBootRegion MainBootRegion = get_mainBootRegion();

	allocationBMapEntry AllocationBMapEntry1;
	allocationBMapEntry AllocationBMapEntry2;
	upCaseTableDirectoryEntry UpCaseTableDirectoryEntry;
	volumeLabelEntry VolumeLabelEntry;


	AllocationBMapEntry1.EntryType = 0x81;
	AllocationBMapEntry1.bitmapFlags = 0x00;
	AllocationBMapEntry1.firstCluster = 2;				//First bitmap table will take 1 cluster of 32kB
	AllocationBMapEntry1.dataLength = 4588;

	AllocationBMapEntry2.EntryType = 0x81;
	AllocationBMapEntry2.bitmapFlags = 0x01;
	AllocationBMapEntry2.firstCluster = 3;				//Second bitmap table will be right next to first bitmap
	AllocationBMapEntry2.dataLength = 4588;

	UpCaseTableDirectoryEntry.EntryType = 0x82;			
	UpCaseTableDirectoryEntry.firstCluster = 4;
	UpCaseTableDirectoryEntry.dataLength = 128;

	VolumeLabelEntry.EntryType = 0x83;
	VolumeLabelEntry.volumeLabel[0] = 'N';

	fseek(volume, (MainBootRegion.clusterHeapOffset*SECTOR_SIZE)+(MainBootRegion.rootDirectoryFirstCluster*CLUSTERSIZE), 0);

	// fwrite()
}
