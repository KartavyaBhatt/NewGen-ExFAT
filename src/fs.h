#ifndef FS_H
#define FS_H

#include <inttypes.h>

typedef struct allocationBMapEntry
{
	uint8_t EntryType;
	uint8_t bitmapFlags;
	uint8_t reserved[18];
	uint32_t firstCluster;
	uint64_t dataLength;
}allocationBMapEntry;


typedef struct upCaseTableDirectoryEntry
{
	uint8_t EntryType;
	uint8_t reserved1[3];
	uint32_t tableChecksum;
	uint8_t reserved2[12];
	uint32_t firstCluster;
	uint64_t dataLength;
}upCaseTableDirectoryEntry;

typedef struct volumeLabelEntry
{
	uint8_t EntryType;
	uint8_t charCount;
	char volumeLabel[22];
	uint8_t reserved[8];
}volumeLabelEntry;

typedef struct fileDirectoryEntry
{
	uint8_t EntryType;
	uint8_t secondaryCount;
	uint16_t setChecksum;
	uint16_t fileAttribute;
	uint8_t reserved1[2];
	uint32_t createTimestamp;
	uint32_t lastModifiedTimestamp;
	uint32_t lastAccessedTimestamp;
	uint8_t create10msIncreament;
	uint8_t lastModified10msIncreament;
	uint8_t createTZoffset;
	uint8_t lastModifiedTZoffset;
	uint8_t lastAccessedTZoffset;
	uint8_t reserved2[7];
}fileDirectoryEntry;

typedef struct streamExtensionEntry
{
	uint8_t EntryType;
	uint8_t generalSecondaryFlags;
	uint8_t reserved1;
	uint8_t nameLength;
	uint16_t nameHash;
	uint16_t reserved2;
	uint64_t vaildDataLength;
	uint32_t reserved3;
	uint32_t firstCluster;
	uint64_t dataLength; 
}streamExtensionEntry;


typedef struct fileNameEntry
{
	uint8_t EntryType;
	uint8_t generalSecondaryFlags;
	char fileName[30];
}fileNameEntry;

#endif