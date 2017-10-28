#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileSystem.h"

static const long long int volumeSize;
static const int culsterSize;
static FILE *volume;

static int NGEF_init(const char *partition_name, long long int size)
{
		volume = fopen(partition_name, "w");

		if (volume == NULL)
			return 0;

		fclose(volume);

		volume = fopen(partition_name, "a+");

		ftruncate(fileno(volume), size);
		
		volumeSize = size;

		culsterSize = (volumeSize >= 7340032 && volumeSize <= 268435456 ? 4096 : (volumeSize > 268435456 && volumeSize <= 34359720776 ? 32768 : (volumeSize > 34359720776 && volumeSize <= 281475070097329 ? 131072 : 0)));

		return 1;
}

static int mount_NGFS()
{

	main_boot_region_init();
	backup_boot_region_init();
	fat1_init();
	fat2_init();

	return 1;
}

static void main_boot_region_init()
{
	//Boot Sector
	char jumpBoot[3];
	char fileSystemName[8];
	char mustBeZero[53];
	char partitionOffset[8];
	char volumeLength[8];
	char fatOffset[4];
	char fatLength[4];
	char clusterHeapOffset[4];
	char clusterCount[4];
	char rootDirectoryFirstCluster[4];
	char volumeSerialNumber[4];
	char fileSystemRevision[2];
	char volumeFlags[2];
	char bytesPerSector[1] = 0b00001001;
	char sectorsPerCluster[1];
	char numberofFATS[1];
	char driveSelect[1];
	char percentInUse[1];
	char reserved[7];
	char bootCode[390];
	char bootSignature[2] = 0xAA55;

	char mainBootSector[SECTOR_SIZE];
	snprintf(mainBootSector, sizeof(mainBootSector), "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", jumpBoot, fileSystemName, mustBeZero, partitionOffset, volumeLength, fatOffset, fatLength, clusterHeapOffset, clusterCount, rootDirectoryFirstCluster, volumeSerialNumber, fileSystemRevision, volumeFlags, bytesPerSector, sectorsPerCluster, numberofFATS, driveSelect, percentInUse, reserved, bootCode, bootSignature);

	//Extended boot sector
	char extendedBootCode[8*SECTOR_SIZE - 4];
	char extendedBootSignature[4] = 0xAA550000;

	char extendedBootSector[4096];
	snprintf(extendedBootSector, sizeof(extendedBootSector), "%s%s", extendedBootCode, extendedBootSignature);

	//OEM Parameters
	char oemParameters[508];
	char checkSum[4];

	char OEMParameters[SECTOR_SIZE];
	snprintf(OEMParameters, sizeof(OEMParameters), "%s%s", oemParameters, checkSum);

	//Reserved Sector
	char reservedSector[SECTOR_SIZE];
	memset(reservedSector, 0, SECTOR_SIZE);

	//VBR Hash
	char vbrHash[SECTOR_SIZE];


	char buffer[12*SECTOR_SIZE];
	snprintf(buffer, sizeof(buffer), "%s%s%s%s", mainBootSector, extendedBootSector, OEMParameters, reservedSector, vbrHash);

	for (int i = 0; buffer[i] != NULL; i++)
	{
		putc(buffer[i], volume);
	}
}

static void backup_boot_region_init()
{
	//Boot Sector
	char jumpBoot[3];
	char fileSystemName[8];
	char mustBeZero[53];
	char partitionOffset[8];
	char volumeLength[8];
	char fatOffset[4];
	char fatLength[4];
	char clusterHeapOffset[4];
	char clusterCount[4];
	char rootDirectoryFirstCluster[4];
	char volumeSerialNumber[4];
	char fileSystemRevision[2];
	char volumeFlags[2];
	char bytesPerSector[1] = 0b00001001;
	char sectorsPerCluster[1];
	char numberofFATS[1];
	char driveSelect[1];
	char percentInUse[1];
	char reserved[7];
	char bootCode[390];
	char bootSignature[2] = 0xAA55;

	char mainBootSector[SECTOR_SIZE];
	snprintf(mainBootSector, sizeof(mainBootSector), "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", jumpBoot, fileSystemName, mustBeZero, partitionOffset, volumeLength, fatOffset, fatLength, clusterHeapOffset, clusterCount, rootDirectoryFirstCluster, volumeSerialNumber, fileSystemRevision, volumeFlags, bytesPerSector, sectorsPerCluster, numberofFATS, driveSelect, percentInUse, reserved, bootCode, bootSignature);

	//Extended boot sector
	char extendedBootCode[8*SECTOR_SIZE - 4];
	char extendedBootSignature[4] = 0xAA550000;

	char extendedBootSector[8*SECTOR_SIZE];
	snprintf(extendedBootSector, sizeof(extendedBootSector), "%s%s", extendedBootCode, extendedBootSignature);

	//OEM Parameters
	char oemParameters[508];
	char checkSum[4];

	char OEMParameters[SECTOR_SIZE];
	snprintf(OEMParameters, sizeof(OEMParameters), "%s%s", oemParameters, checkSum);

	//Reserved Sector
	char reservedSector[SECTOR_SIZE];
	memset(reservedSector, 0, SECTOR_SIZE);

	//VBR Hash
	char vbrHash[SECTOR_SIZE];


	char buffer[12*SECTOR_SIZE];
	snprintf(buffer, sizeof(buffer), "%s%s%s%s", mainBootSector, extendedBootSector, OEMParameters, reservedSector, vbrHash);

	for (int i = 0; buffer[i] != NULL; i++)
	{
		putc(buffer[i], volume);
	}
}

static void fat1_init()
{

}

static void fat2_init()
{

}