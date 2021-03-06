#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "io.h"

#define SECTOR_SIZE 512
#define CLUSTERCOUNT 36700
#define FATOFFSET 65*SECTOR_SIZE
#define FATLEGTH 256*SECTOR_SIZE
#define CLUSTERSIZE 32768

static bool NGEF_init(const char *partition_name, long long int size);
static bool mount_NGEF();
static bool fat2_init();
static bool fat1_init();
static bool main_boot_region_init();
static bool backup_boot_region_init();
static bool clusterHeap_init();
static FILE* get_volume();
static void fat1_ret();
static void fat2_ret();
static bool bitMapTable_init();
static void bitMapTable1_ret();
static void bitMapTable2_ret();
static bool upCaseTable_init();

typedef struct mainBootRegion 
{
	uint8_t jumpBoot[3];
	uint8_t fileSystemName[8];
	uint8_t mustBeZero[53];
	uint8_t partitionOffset[8];
	uint8_t volumeLength[8];
	uint32_t fatOffset;
	uint32_t fatLength;
	uint32_t clusterHeapOffset;
	uint32_t clusterCount;
	uint32_t rootDirectoryFirstCluster;
	uint32_t volumeSerialNumber;
	uint16_t fileSystemRevision;
	uint16_t volumeFlags;
	uint8_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint8_t numberofFATS;
	uint8_t driveSelect;
	uint8_t percentInUse;
	uint8_t reserved[7];
	uint8_t bootCode[390];
	uint16_t bootSignature;

	//Extended boot sector
	uint8_t extendedBootCode[8*SECTOR_SIZE - 4];
	uint8_t extendedBootSignature[4];

	//OEM Parameters
	uint8_t oemParameters[508];
	uint8_t checkSum[4];

	//Reserved Sector
	uint8_t reservedSector[SECTOR_SIZE];

	//VBR Hash
	uint8_t vbrHash[SECTOR_SIZE];
} mainBootRegion;

static mainBootRegion get_mainBootRegion();	

typedef struct backupBootRegion
{
	uint8_t jumpBoot[3];
	uint8_t fileSystemName[8];
	uint8_t mustBeZero[53];
	uint8_t partitionOffset[8];
	uint8_t volumeLength[8];
	uint32_t fatOffset;
	uint32_t fatLength;
	uint32_t clusterHeapOffset;
	uint32_t clusterCount;
	uint32_t rootDirectoryFirstCluster;
	uint32_t volumeSerialNumber;
	uint16_t fileSystemRevision;
	uint16_t volumeFlags;
	uint8_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint8_t numberofFATS;
	uint8_t driveSelect;
	uint8_t percentInUse;
	uint8_t reserved[7];
	uint8_t bootCode[390];
	uint16_t bootSignature;

	//Extended boot sector
	uint8_t extendedBootCode[8*SECTOR_SIZE - 4];
	uint8_t extendedBootSignature[4];

	//OEM Parameters
	uint8_t oemParameters[508];
	uint8_t checkSum[4];

	//Reserved Sector
	uint8_t reservedSector[SECTOR_SIZE];

	//VBR Hash
	uint8_t vbrHash[SECTOR_SIZE];
}backupBootRegion;

typedef struct fatTable
{
	uint32_t fatEntry[CLUSTERCOUNT];

}fatTable;

typedef struct tFatTable
{
	uint32_t fatEntry[CLUSTERCOUNT];
}tFatTable;

typedef struct cluster
{
	uint8_t data[CLUSTERSIZE];	
}cluster;

typedef struct bitMapTable
{
	uint8_t bitMap[4588];
}bitMapTable;

typedef struct upCaseTable
{
	uint16_t upCaseEntry[128];
}upCaseTable;

static bitMapTable get_bitmapTable1();
static bitMapTable get_bitmapTable2();
static void set_bitMapTable1(bitMapTable BitMapTable);
static void set_bitMapTable2(bitMapTable BitMapTable);


#endif