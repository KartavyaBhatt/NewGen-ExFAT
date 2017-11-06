#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <inttypes.h>

#define SECTOR_SIZE 512
#define CLUSTERCOUNT 36700
#define FATOFFSET 65*SECTOR_SIZE
#define FATLEGTH 256*SECTOR_SIZE

static bool NGEF_init(const uint8_t *partition_name, long long int size);
static bool mount_NGFS();
static bool fat2_init();
static bool fat1_init();
static void main_boot_region_init();
static void backup_boot_region_init();

typedef struct mainBootRegion 
{
	uint8_t jumpBoot[3];
	uint8_t fileSystemName[8];
	uint8_t mustBeZero[53];
	uint8_t partitionOffset[8];
	uint8_t volumeLength[8];
	uint8_t fatOffset[4] = 65;
	uint8_t fatLength[4] = 256;
	uint8_t clusterHeapOffset[4] = 580;
	uint8_t clusterCount[4];
	uint8_t rootDirectoryFirstCluster[4] = 2;
	uint8_t volumeSerialNumber[4];
	uint8_t fileSystemRevision[2];
	uint8_t volumeFlags[2];
	uint8_t bytesPerSector = 9;
	uint8_t sectorsPerCluster;
	uint8_t numberofFATS = 2;
	uint8_t driveSelect[1];
	uint8_t percentInUse[1];
	uint8_t reserved[7];
	uint8_t bootCode[390];
	uint8_t bootSignature[2];

	//Extended boot sector
	uint8_t extendedBootCode[8*SECTOR_SIZE - 4];
	uint8_t extendedBootSignature[4];

	//OEM Parameters
	uint8_t oemParameters[508];
	uint8_t checkSum[4];

	//Reserved Sector
	uint8_t reservedSector[SECTOR_SIZE];
	memset(reservedSector, 0, SECTOR_SIZE);

	//VBR Hash
	uint8_t vbrHash[SECTOR_SIZE];
} mainBootRegion;

typedef struct backupBootRegion
{
	uint8_t jumpBoot[3];
	uint8_t fileSystemName[8];
	uint8_t mustBeZero[53];
	uint8_t partitionOffset[8];
	uint8_t volumeLength[8];
	uint8_t fatOffset[4] = 65;
	uint8_t fatLength[4] = 256;
	uint8_t clusterHeapOffset[4] = 580;
	uint8_t clusterCount[4];
	uint8_t rootDirectoryFirstCluster[4] = 2;
	uint8_t volumeSerialNumber[4];
	uint8_t fileSystemRevision[2];
	uint8_t volumeFlags[2];
	uint8_t bytesPerSector = 9;
	uint8_t sectorsPerCluster;
	uint8_t numberofFATS = 2;
	uint8_t driveSelect[1];
	uint8_t percentInUse[1];
	uint8_t reserved[7];
	uint8_t bootCode[390];
	uint8_t bootSignature[2];

	//Extended boot sector
	uint8_t extendedBootCode[8*SECTOR_SIZE - 4];
	uint8_t extendedBootSignature[4];

	//OEM Parameters
	uint8_t oemParameters[508];
	uint8_t checkSum[4];

	//Reserved Sector
	uint8_t reservedSector[SECTOR_SIZE];
	memset(reservedSector, 0, SECTOR_SIZE);

	//VBR Hash
	uint8_t vbrHash[SECTOR_SIZE];
}backupBootRegion;

typedef struct fatTable
{
	uint32_t fatEntry[CLUSTERCOUNT];

}fatTable1;

typedef struct tFatTable
{
	uint32_t fatEntry[CLUSTERCOUNT];
}tFatTable;

#endif