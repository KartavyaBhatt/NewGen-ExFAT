#include <stdio.h>
#include <inttypes.h>
#include <string.h>

union convert16{
	uint32_t x;
	uint8_t y[2];
}convert16;

union convert32{
	uint32_t x;
	uint8_t y[4];
}convert32;

union convert64{
	uint64_t x;
	uint8_t y[8];
}convert64;


void cti_allocationBMapEntry(allocationBMapEntry AllocationBMapEntry, uint8_t * str)
{
	uint8_t tmp8;

	str[0] = AllocationBMapEntry.EntryType;
	
	str[1] = AllocationBMapEntry.bitmapFlags;

	for (int i = 0; i < 18; i++)
		str[i+2] = 0;

	convert32.x = AllocationBMapEntry.firstCluster;	
	
	for (int i = 0; i < 4; i++)
		str[20+i] = convert32.y[0+i];
	
	convert64.x = AllocationBMapEntry.dataLength;
	for (int i = 0; i < 8; i++)
		str[24+i] = convert64.y[0+i];

	for (int i = 0; i < 8; ++i)
	{
		printf("String : %" PRIu8 "\n", str[i+24]);
	}
}

allocationBMapEntry cts_allocationBMapEntry(uint8_t * str)
{
	allocationBMapEntry AllocationBMapEntry;

	AllocationBMapEntry.EntryType = str[0];

	AllocationBMapEntry.bitmapFlags =str[1] ;

	for (int i = 0; i < 18; ++i)
	{
		AllocationBMapEntry.reserved[i] = str[2+i];
	}

	for (int i = 0; i < 4; i++)
	 	convert32.y[0+i] = str[20+i];
	
	AllocationBMapEntry.firstCluster = convert32.x;

	for (int i = 0; i < 8; i++)
		convert64.y[0+i] = str[24+i];
	
	AllocationBMapEntry.dataLength = convert64.x;

	return AllocationBMapEntry;
}

void cti_upCaseTableDirectoryEntry(upCaseTableDirectoryEntry UpCaseTableDirectoryEntry, uint8_t * str)
{
	 str[0] = UpCaseTableDirectoryEntry.EntryType;

	for (int i = 0; i < 3; i++)
		str[i+2] = 0;

	convert32.x = UpCaseTableDirectoryEntry.tableChecksum;
	
	for (int i = 0; i < 4; i++)
		str[4+i] = convert32.y[0+i];

	for (int i = 0; i < 12; ++i)
		str[8+i] = 0;

	convert32.x = UpCaseTableDirectoryEntry.firstCluster;
	
	for (int i = 0; i < 4; i++)
		str[20+i] = convert32.y[0+i];
	
	convert64.x = UpCaseTableDirectoryEntry.dataLength;
	for (int i = 0; i < 8; i++)
		str[24+i] = convert64.y[0+i];
}

upCaseTableDirectoryEntry cts_upCaseTableDirectoryEntry (uint8_t * str)
{
	upCaseTableDirectoryEntry UpCaseTableDirectoryEntry;

	UpCaseTableDirectoryEntry.EntryType = str[0];

	for (int i = 0; i < 4; i++)
	 	convert32.y[0+i] = str[4+i];
	
	UpCaseTableDirectoryEntry.tableChecksum = convert32.x;

	for (int i = 0; i < 4; i++)
	 	convert32.y[0+i] = str[20+i];
	
	UpCaseTableDirectoryEntry.firstCluster = convert32.x;


	for (int i = 0; i < 8; i++)
		convert64.y[0+i] = str[24+i];
	
	UpCaseTableDirectoryEntry.dataLength = convert64.x;

	return UpCaseTableDirectoryEntry;
}


void cti_volumeLabelEntry(volumeLabelEntry VolumeLabelEntry, uint8_t * str)
{
	char tmp8;

	str[0] = VolumeLabelEntry.EntryType;
	
	str[1] = VolumeLabelEntry.charCount;

	for (int i = 0; i < 22; ++i)
		str[2+i] = VolumeLabelEntry.volumeLabel[i];
}

volumeLabelEntry cts_volumeLabelEntry (uint8_t * str)
{
	volumeLabelEntry VolumeLabelEntry;

	VolumeLabelEntry.EntryType = str[0];

	VolumeLabelEntry.charCount = str[1];

	for (int i = 0; i < 22; i++)
	 	VolumeLabelEntry.volumeLabel[0+i] = str[2+i];
	
	return VolumeLabelEntry;
}


void cti_fileDirectoryEntry(fileDirectoryEntry FileDirectoryEntry, uint8_t * str)
{
	str[0] = FileDirectoryEntry.EntryType;
	
	str[1] = FileDirectoryEntry.secondaryCount;
	
	convert16.x = FileDirectoryEntry.setChecksum;
	for (int i = 0; i < 2; ++i)
		str[2+i] = convert16.y[i];

	convert16.x = FileDirectoryEntry.fileAttribute;
	for (int i = 0; i < 2; ++i)
		str[4+i] = convert16.y[i];

	for (int i = 0; i < 2; ++i)
		str[6+i] = 0;

	convert32.x = FileDirectoryEntry.createTimestamp;
	for (int i = 0; i < 4; ++i)
		str[8+i] = convert32.y[i];

	convert32.x FileDirectoryEntry.lastModifiedTimestamp[i];;
	for (int i = 0; i < 4; ++i)
		str[12+i] = convert32.y[i];

	convert32.x = FileDirectoryEntry.lastAccessedTimestamp;
	for (int i = 0; i < 4; ++i)
		str[16+i] = convert32.y[i];

	str[20] = FileDirectoryEntry.create10msIncreament;

	str[21] = FileDirectoryEntry.lastModified10msIncreament;

	str[22] = FileDirectoryEntry.createTZoffset;

	str[23] = FileDirectoryEntry.lastModifiedTZoffset;

	str[24] = FileDirectoryEntry.lastAccessedTZoffset;

	for (int i = 0; i < 7; ++i)
		str[25+i] = 0;
}

fileDirectoryEntry cts_fileDirectoryEntry (uint8_t * str)
{
	fileDirectoryEntry FileDirectoryEntry;
	char tmp8;
	char tmp16[2];
	char tmp32[4];

	strncpy(&tmp8, str + 0, 1);
	FileDirectoryEntry.EntryType = convert_to_int8(&tmp8);

	strncpy(&tmp8, str + 1, 1);
	FileDirectoryEntry.secondaryCount = convert_to_int8(&tmp8);

	for (int i = 0; i < 2; i++)
	 	tmp16[0+i] = str[2+i];

	FileDirectoryEntry.setChecksum = convert_to_int16(tmp16);

	for (int i = 0; i < 2; i++)
	 	tmp16[0+i] = str[4+i];

	FileDirectoryEntry.fileAttribute = convert_to_int16(tmp16);	

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[8+i];

	FileDirectoryEntry.createTimestamp = convert_to_int32(tmp32);

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[12+i];

	FileDirectoryEntry.lastModifiedTimestamp = convert_to_int32(tmp32);

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[16+i];

	FileDirectoryEntry.lastAccessedTimestamp = convert_to_int32(tmp32);	

	FileDirectoryEntry.create10msIncreament = convert_to_int8(&str[20]);

	FileDirectoryEntry.lastModified10msIncreament = convert_to_int8(&str[21]);

	FileDirectoryEntry.createTZoffset = convert_to_int8(&str[22]);

	FileDirectoryEntry.lastModifiedTZoffset = convert_to_int8(&str[23]);

	FileDirectoryEntry.lastAccessedTZoffset = convert_to_int8(&str[24]);

	return FileDirectoryEntry;
}



void cti_streamExtensionEntry(streamExtensionEntry StreamExtensionEntry, uint8_t * str)
{
	str[0] = StreamExtensionEntry.EntryType;
	
	str[1] = StreamExtensionEntry.generalSecondaryFlags;
	
	str[2] = 0;

	str[3] = StreamExtensionEntry.nameLength;

	convert16.x = StreamExtensionEntry.nameHash;
	for (int i = 0; i < 2; ++i)
		str[4+i] = convert16.y[i];

	for (int i = 0; i < 2; ++i)
		str[6+i] = 0;

	convert64.x = StreamExtensionEntry.vaildDataLength;
	for (int i = 0; i < 8; ++i)
		str[8+i] = convert64.y[i];

	for (int i = 0; i < 4; ++i)
		str[16+i] = 0;

	convert32.x = StreamExtensionEntry.firstCluster;
	for (int i = 0; i < 4; ++i)
		str[20+i] = convert32.y[i];

	convert64.x = StreamExtensionEntry.dataLength;
	for (int i = 0; i < 8; ++i)
		str[24+i] = convert64.y[i];
}

streamExtensionEntry cts_streamExtensionEntry (uint8_t * str)
{
	streamExtensionEntry StreamExtensionEntry;

	StreamExtensionEntry.EntryType = str[0];

	StreamExtensionEntry.generalSecondaryFlags = str[1];

	StreamExtensionEntry.nameLength = str[3];

	for (int i = 0; i < 2; i++)
	 	convert16.y[0+i] = str[4+i];

	StreamExtensionEntry.nameHash = convert16.x;	

	for (int i = 0; i < 8; i++)
	 	convert64.y[0+i] = str[8+i];

	StreamExtensionEntry.vaildDataLength = convert64.x;

	for (int i = 0; i < 4; i++)
	 	convert32.y[0+i] = str[20+i];

	StreamExtensionEntry.firstCluster = convert32.x;

	for (int i = 0; i < 8; i++)
	 	convert64.y[0+i] = str[24+i];

	StreamExtensionEntry.dataLength = convert64.x;	

	return StreamExtensionEntry;
}


void cti_fileNameEntry(fileNameEntry FileNameEntry, uint8_t * str)
{
	str[0] = FileNameEntry.EntryType;

	str[1] = FileNameEntry.generalSecondaryFlags;

	for (int i = 0; i < 30; i++)
		str[2+i] = FileNameEntry.fileName[i];
}

fileNameEntry cts_fileNameEntry(uint8_t * str)
{
	fileNameEntry FileNameEntry;

	FileNameEntry.EntryType = str[0];
	FileNameEntry.generalSecondaryFlags = str[1];

	for (int i = 0; i < 30; i++)
		FileNameEntry.fileName[i] = str[2+i];

	return FileNameEntry;
}