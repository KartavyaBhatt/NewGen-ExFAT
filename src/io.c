#include <stdio.h>
#include <string.h>
#include "partition.h"
#include "conversion.c"

bool write_cluster(uint8_t *data, long int clusterNumber)
{
	mainBootRegion MainBootRegion = get_mainBootRegion();
	
	if (clusterNumber <= MainBootRegion.clusterCount && data != NULL)
	{
		FILE *volume = get_volume();
		cluster Cluster;
		

		for (int i = 0; i < CLUSTERSIZE; i++)
			Cluster.data[i] = data[i];

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

bool read_cluster(uint8_t *buff, long int clusterNumber)
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
			for (int i = 0; i < CLUSTERSIZE; ++i)
			{
				buff[i] = Cluster.data[i];
			}
			// Debug :: printf("Read Buff Data : %s\n", buff);
			return true;
		}
		else
			return false;
	}
}


uint32_t get_freeCluster()
{
	FILE *volume = get_volume();
	bitMapTable BitMapTable1 = get_bitmapTable1();

	for (int i = 0; i < 4588; ++i)
	{
		if((BitMapTable1.bitMap[i] | 0xFE) == 0xFE)
			return ((8*i)+2)+0;

		else if ((BitMapTable1.bitMap[i] | 0xFD) == 0xFD)
			return ((8*i)+2)+1;

		else if ((BitMapTable1.bitMap[i] | 0xFB) == 0xFB)
			return ((8*i)+2)+2;

		else if ((BitMapTable1.bitMap[i] | 0xF7) == 0xF7)
			return ((8*i)+2)+3;

		else if ((BitMapTable1.bitMap[i] | 0xEF) == 0xEF)
			return ((8*i)+2)+4;

		else if ((BitMapTable1.bitMap[i] | 0xDF) == 0xDF)
			return ((8*i)+2)+5;

		else if ((BitMapTable1.bitMap[i] | 0xBF) == 0xBF)
			return ((8*i)+2)+6;

		else if ((BitMapTable1.bitMap[i] | 0x7F) == 0x7F)
			return ((8*i)+2)+7;
	}

	return 0;
}

void set_cluster_bitmap1(uint32_t clusterNumber)
{
	bitMapTable BitMapTable1 = get_bitmapTable1();

	BitMapTable1.bitMap[(uint8_t) ((clusterNumber-2)/8)] = BitMapTable1.bitMap[(uint8_t) ((clusterNumber-2)/8)] | (1  >> (7-((clusterNumber-2)%8)));

	set_bitMapTable1(BitMapTable1);
}

void set_cluster_bitmap2(uint32_t clusterNumber)
{
	bitMapTable BitMapTable1 = get_bitmapTable2();

	BitMapTable1.bitMap[(uint8_t) ((clusterNumber-2)/8)] = BitMapTable1.bitMap[(uint8_t) ((clusterNumber-2)/8)] | (1  >> (7-((clusterNumber-2)%8)));

	set_bitMapTable2(BitMapTable1);
}

static bool rootDir_init()
{
	FILE *volume = get_volume();
	mainBootRegion MainBootRegion = get_mainBootRegion();

	allocationBMapEntry AllocationBMapEntry1;
	allocationBMapEntry AllocationBMapEntry2;
	upCaseTableDirectoryEntry UpCaseTableDirectoryEntry;
	volumeLabelEntry VolumeLabelEntry;

	uint8_t str[CLUSTERSIZE];
	uint8_t arr[32];

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

	cti_allocationBMapEntry(AllocationBMapEntry1, arr);
	for (int i = 0; i < 32; ++i)
		str[i] = arr[i];

	cti_allocationBMapEntry(AllocationBMapEntry2, arr);
	for (int i = 0; i < 32; ++i)
		str[32+i] = arr[i];

	cti_upCaseTableDirectoryEntry(UpCaseTableDirectoryEntry, arr);
	for (int i = 0; i < 32; ++i)
		str[64+i] = arr[i];

	cti_volumeLabelEntry(VolumeLabelEntry, arr);
	for (int i = 0; i < 32; ++i)
		str[96+i] = arr[i];

	if(write_cluster(str, MainBootRegion.rootDirectoryFirstCluster))
		return true;

	else
		return false;
}

