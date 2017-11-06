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

static bool NGEF_init(const char *partition_name, long long int size)
{
		char buff[size];
		printf("Creating partition...");
		volume = fopen(partition_name, "w");

		if (volume == NULL)
		{
			printf("Partition not created due to some error. Please try again...\n");
			return false;
		}

		fclose(volume);

		volume = fopen(partition_name, "a+");

		ftruncate(fileno(volume), size);
		memset(buff, '\0', size);

		volumeSize = size;

		if(volumeSize < 7340032 || volumeSize > 281475070097329)
		{
			perror("Volume Size not supported :( rolling back the changes...");
			return false;
		}
		
		else
			culsterSize = (volumeSize >= 7340032 && volumeSize <= 268435456 ? 4096 : (volumeSize > 268435456 && volumeSize <= 34359720776 ? 32768 : (volumeSize > 34359720776 && volumeSize <= 281475070097329 ? 131072 : 0)));

		printf("Partition created");

		return true;
}

static bool mount_NGFS()
{
	main_boot_region_init();
	backup_boot_region_init();
	fat1_init();
	fat2_init();

	printf("NewGen ExFAT is sucessfully mounted on the partition !");
	return true;
}

static bool main_boot_region_init()
{
	fwrite(&MainBootRegion, sizeof(mainBootRegion), 1, volume);

	if (fwrite)
		return true;
	else
		return false;
}

static bool backup_boot_region_init()
{
	fseek(volume, FATLEGTH, 0);
	fwrite(&BackupBootRegion, sizeof(backupBootRegion), 1, volume);

	if (fwrite)
		return true;
	else
		return false;
}

//Creating 32700 clusters of size 32kB so the fat table will have 32700 entries 

static bool fat1_init()
{
	fseek(volume, FATOFFSET,0);
	fwrite(&FatTable, sizeof(FatTable), 1, volume);

	return true;
}

static bool fat2_init()
{
	fseek(volume, FATOFFSET+FATLEGTH, 0);
	fwrite(&TFatTable, sizeof(TFatTable), 1, volume);
	return true;
}
