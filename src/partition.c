#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>
#include "partition.h"

static long long int volumeSize;
static int culsterSize;
static FILE *volume;
static mainBootRegion MainBootRegion;
static backupBootRegion BackupBootRegion;
static fatTable FatTable;
static tFatTable TFatTable;
static cluster Cluster[CLUSTERCOUNT];

static bool NGEF_init(const char *partition_name, long long int size)
{
		char buff[size];

		printf("Opening partition...\n");
		volume = fopen(partition_name, "r+");

		if (volume == NULL)
		{
			printf("Partition not present. Creating a new partition...\n");
			volume = fopen(partition_name, "w+");

			if (volume == NULL)
			{
				return false;
			}
		

			ftruncate(fileno(volume), size);

			volumeSize = size;

			if(volumeSize < 7340032 || volumeSize > 281475070097329)
			{
				perror("Volume Size not supported :( rolling back the changes...\n");
				return false;
			}
			
			else
			{
				culsterSize = (volumeSize >= 7340032 && volumeSize <= 268435456 ? 4096 : (volumeSize > 268435456 && volumeSize <= 34359720776 ? 32768 : (volumeSize > 34359720776 && volumeSize <= 281475070097329 ? 131072 : 0)));
			}

			mount_NGEF();
		}
		
		printf("Partition established !\n");

		return true;
}

static bool mount_NGEF()
{
	if(main_boot_region_init() && backup_boot_region_init() && fat1_init() && fat2_init() && clusterHeap_init())
	{
		printf("NewGen ExFAT is sucessfully mounted on the partition !\n");
		return true;
	}

	else
	{
		printf("Some error occured. Unable to mount file system.\n");
		return false;
	}
}

static bool main_boot_region_init()
{
	// Debug :: long int z;
	MainBootRegion.clusterHeapOffset = 580;
	MainBootRegion.fatOffset = 65;
	MainBootRegion.fatLength = 256;
	MainBootRegion.rootDirectoryFirstCluster = 2;
	MainBootRegion.bytesPerSector = 9;
	MainBootRegion.clusterCount = 36700;

	fseek(volume, 0, 0);
	// Debug :: z = ftell(volume);
	// Debug :: printf("MainBootRegion : %ld\n", z);

	if (fwrite(&MainBootRegion, sizeof(mainBootRegion), 1, volume) == 1)
	{
		return true;
	}
	else
		return false;
}

static bool backup_boot_region_init()
{
	// Debug :: long int z;

	BackupBootRegion.clusterHeapOffset = 580;
	BackupBootRegion.fatOffset = 65;
	BackupBootRegion.fatLength = 256;
	BackupBootRegion.rootDirectoryFirstCluster = 2;
	BackupBootRegion.bytesPerSector = 9;
	BackupBootRegion.clusterCount = 36700;

	fseek(volume, sizeof(MainBootRegion)*SECTOR_SIZE, 0);
	// Debug :: z = ftell(volume);
	// Debug :: printf("BackupBootRegion : %ld\n", z);

	if (fwrite(&BackupBootRegion, sizeof(backupBootRegion), 1, volume) == 1)
		return true;
	else
		return false;
}

//Creating 32700 clusters of size 32kB so the fat table will have 32700 entries 

static bool fat1_init()
{
	// Debug :: long int z;

	fseek(volume, MainBootRegion.fatOffset*SECTOR_SIZE,0);
	// Debug :: z = ftell(volume);
	// Debug :: printf("FatTable : %ld\n", z);

	if (fwrite(&FatTable, sizeof(FatTable), 1, volume) == 1)
		return true;

	else
		return false;
}

static bool fat2_init()
{
	// Debug :: long int z;

	fseek(volume, (MainBootRegion.fatOffset+MainBootRegion.fatLength)*SECTOR_SIZE, 0);
	// Debug :: z = ftell(volume);
	// Debug :: printf("TFatTable: %ld\n", z);
	
	if (fwrite(&TFatTable, sizeof(TFatTable), 1, volume) == 1)
		return true;

	else
		return false;
}

static bool clusterHeap_init()
{
	// Debug :: long int z;

	fseek(volume, MainBootRegion.clusterHeapOffset*SECTOR_SIZE, 0);
	// Debug :: z = ftell(volume);
	// Debug :: printf("ClusterHeap : %ld\n", z);


	for (int i = 0; i < MainBootRegion.clusterCount; ++i)
	{
		if (fwrite(&Cluster[i], sizeof(Cluster[i]), 1, volume) != 1)
			return false;	
	}

	// Debug :: z = ftell(volume);
	// Debug :: printf("ClusterHeap End: %ld\n", z);
	return true;
}

static FILE* get_volume()
{
	return volume;
}

static mainBootRegion get_mainBootRegion()
{
	return MainBootRegion;
}