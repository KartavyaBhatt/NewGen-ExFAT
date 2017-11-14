#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "conversion.h"

void convert_to_char8(uint8_t z, unsigned char * const ser)
{
	ser[0] = z;
}

uint8_t convert_to_int8(unsigned char * const ser)

{
	uint8_t z = 0;

	z = (z << 8) | ser[0];

	return z;
}

void convert_to_char16(uint16_t z, unsigned char * const ser)
{
	ser[0] = z >> 8;
	ser[1] = z;
}

uint16_t convert_to_int16(unsigned char * const ser)
{
	uint16_t z = 0;

	z = (z << 8) | ser[0];
	z = (z << 8) | ser[1];

	return z;
}

void convert_to_char32(uint32_t z, unsigned char * const ser)
{
	ser[0] = z >> 24;
	ser[1] = z >> 16;
	ser[2] = z >> 8;
	ser[3] = z;
}

uint32_t convert_to_int32(unsigned char * const ser)
{
	uint32_t z = 0;

	z = (z << 8) | ser[0];
	z = (z << 8) | ser[1];
	z = (z << 8) | ser[2];
	z = (z << 8) | ser[3];
	
	return z;
}

void convert_to_char64(uint64_t z, unsigned char * const ser)
{
	ser[0] = z >> 56;
	ser[1] = z >> 48;
	ser[2] = z >> 40;
	ser[3] = z >> 32;
	ser[4] = z >> 24;
	ser[5] = z >> 16;
	ser[6] = z >> 8;
	ser[7] = z;
}

uint64_t convert_to_int64(unsigned char * const ser)
{
	uint64_t z = 0;

	z = (z << 8) | ser[0];
	z = (z << 8) | ser[1];
	z = (z << 8) | ser[2];
	z = (z << 8) | ser[3];
	z = (z << 8) | ser[4];
	z = (z << 8) | ser[5];
	z = (z << 8) | ser[6];
	z = (z << 8) | ser[7];

	return z;
}

void ctc_allocationBMapEntry(allocationBMapEntry AllocationBMapEntry, unsigned char * const str)
{
	char tmp8;
	char tmp32[4];
	char tmp64[8];

	convert_to_char8(AllocationBMapEntry.EntryType, &tmp8);
	strcat(str, &tmp8);
	
	convert_to_char8(AllocationBMapEntry.bitmapFlags, &tmp8);
	strcat(str, &tmp8);

	for (int i = 0; i < 18; i++)
		str[i+2] = 0;

	convert_to_char32(AllocationBMapEntry.firstCluster, tmp32);	
	
	for (int i = 0; i < 4; i++)
		str[20+i] = tmp32[0+i];
	
	convert_to_char64(AllocationBMapEntry.dataLength, tmp64);
	for (int i = 0; i < 8; i++)
		str[24+i] = tmp64[0+i];
}

allocationBMapEntry cts_allocationBMapEntry(unsigned char * const str)
{
	allocationBMapEntry AllocationBMapEntry;
	char tmp8;
	char tmp32[4];
	char tmp64[8];

	strncpy(&tmp8, str + 0, 1);
	AllocationBMapEntry.EntryType = convert_to_int8(&tmp8);

	strncpy(&tmp8, str + 1, 1);
	AllocationBMapEntry.bitmapFlags = convert_to_int8(&tmp8);	

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[20+i];
	
	AllocationBMapEntry.firstCluster = convert_to_int32(tmp32);

	for (int i = 0; i < 8; i++)
		tmp64[0+i] = str[24+i];
	
	AllocationBMapEntry.dataLength = convert_to_int64(tmp64);

	return AllocationBMapEntry;
}

void ctc_upCaseTableDirectoryEntry(upCaseTableDirectoryEntry UpCaseTableDirectoryEntry, unsigned char * const str)
{
	char tmp8;
	char tmp32[4];
	char tmp64[8];

	convert_to_char8(UpCaseTableDirectoryEntry.EntryType, &tmp8);
	strcat(str, &tmp8);
	
	for (int i = 0; i < 3; i++)
		str[i+2] = 0;

	convert_to_char32(UpCaseTableDirectoryEntry.tableChecksum, tmp32);
	
	for (int i = 0; i < 4; i++)
		str[4+i] = tmp32[0+i];

	for (int i = 0; i < 12; ++i)
		str[8+i] = 0;

	convert_to_char32(UpCaseTableDirectoryEntry.firstCluster, tmp32);
	
	for (int i = 0; i < 4; i++)
		str[20+i] = tmp32[0+i];

	
	convert_to_char64(UpCaseTableDirectoryEntry.dataLength, tmp64);
	for (int i = 0; i < 8; i++)
		str[24+i] = tmp64[0+i];
}

upCaseTableDirectoryEntry cts_upCaseTableDirectoryEntry (unsigned char * const str)
{
	upCaseTableDirectoryEntry UpCaseTableDirectoryEntry;
	char tmp8;
	char tmp32[4];
	char tmp64[8];

	strncpy(&tmp8, str + 0, 1);
	UpCaseTableDirectoryEntry.EntryType = convert_to_int8(&tmp8);

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[4+i];
	
	UpCaseTableDirectoryEntry.tableChecksum = convert_to_int32(tmp32);

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[20+i];
	
	UpCaseTableDirectoryEntry.firstCluster = convert_to_int32(tmp32);


	for (int i = 0; i < 8; i++)
		tmp64[0+i] = str[24+i];
	
	UpCaseTableDirectoryEntry.dataLength = convert_to_int64(tmp64);

	return UpCaseTableDirectoryEntry;
}


void ctc_volumeLabelEntry(volumeLabelEntry VolumeLabelEntry, unsigned char * const str)
{
	char tmp8;

	convert_to_char8(VolumeLabelEntry.EntryType, &tmp8);
	strcat(str, &tmp8);
	
	convert_to_char32(VolumeLabelEntry.charCount, &tmp8);
	strcat(str, &tmp8);
	
	for (int i = 0; i < 22; ++i)
		str[2+i] = VolumeLabelEntry.volumeLabel[i];
}

volumeLabelEntry cts_volumeLabelEntry (unsigned char * const str)
{
	volumeLabelEntry VolumeLabelEntry;
	char tmp8;

	strncpy(&tmp8, str + 0, 1);
	VolumeLabelEntry.EntryType = convert_to_int8(&tmp8);

	strncpy(&tmp8, str + 1, 1);
	VolumeLabelEntry.charCount = convert_to_int8(&tmp8);

	for (int i = 0; i < 22; i++)
	 	VolumeLabelEntry.volumeLabel[0+i] = str[2+i];
	
	return VolumeLabelEntry;
}


void ctc_fileDirectoryEntry(fileDirectoryEntry FileDirectoryEntry, unsigned char * const str)
{
	char tmp8;
	char tmp16[2];
	char tmp32[4];

	convert_to_char8(FileDirectoryEntry.EntryType, &tmp8);
	strcat(str, &tmp8);
	
	convert_to_char8(FileDirectoryEntry.secondaryCount, &tmp8);
	strcat(str, &tmp8);
	
	convert_to_char16(FileDirectoryEntry.setChecksum, tmp16);
	for (int i = 0; i < 2; ++i)
		str[2+i] = tmp16[i];

	convert_to_char16(FileDirectoryEntry.fileAttribute, tmp16);
	for (int i = 0; i < 2; ++i)
		str[4+i] = tmp16[i];

	for (int i = 0; i < 2; ++i)
		str[6+i] = 0;

	convert_to_char32(FileDirectoryEntry.createTimestamp, tmp32);
	for (int i = 0; i < 4; ++i)
		str[8+i] = tmp32[i];

	convert_to_char32(FileDirectoryEntry.lastModifiedTimestamp, tmp32);
	for (int i = 0; i < 4; ++i)
		str[12+i] = tmp32[i];

	convert_to_char32(FileDirectoryEntry.lastAccessedTimestamp, tmp32);
	for (int i = 0; i < 4; ++i)
		str[16+i] = tmp32[i];

	convert_to_char32(FileDirectoryEntry.create10msIncreament, &str[20]);

	convert_to_char8(FileDirectoryEntry.lastModified10msIncreament, &str[21]);

	convert_to_char8(FileDirectoryEntry.createTZoffset, &str[22]);

	convert_to_char8(FileDirectoryEntry.lastModifiedTZoffset, &str[23]);

	convert_to_char8(FileDirectoryEntry.lastAccessedTZoffset, &str[24]);

	for (int i = 0; i < 7; ++i)
		str[25+i] = 0;
}

fileDirectoryEntry cts_fileDirectoryEntry (unsigned char * const str)
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



void ctc_streamExtensionEntry(streamExtensionEntry StreamExtensionEntry, unsigned char * const str)
{
	char tmp8;
	char tmp16[2];
	char tmp32[4];
	char tmp64[8];

	convert_to_char8(StreamExtensionEntry.EntryType, &tmp8);
	strcat(str, &tmp8);
	
	convert_to_char8(StreamExtensionEntry.generalSecondaryFlags, &tmp8);
	strcat(str, &tmp8);
	
	str[2] = 0;

	convert_to_char8(StreamExtensionEntry.nameLength, &tmp8);
	str[3] = tmp8;

	convert_to_char16(StreamExtensionEntry.nameHash, tmp16);
	for (int i = 0; i < 2; ++i)
		str[4+i] = tmp16[i];

	for (int i = 0; i < 2; ++i)
		str[6+i] = 0;

	convert_to_char64(StreamExtensionEntry.vaildDataLength, tmp64);
	for (int i = 0; i < 8; ++i)
		str[8+i] = tmp32[i];

	for (int i = 0; i < 4; ++i)
		str[16+i] = 0;

	convert_to_char32(StreamExtensionEntry.firstCluster, tmp32);
	for (int i = 0; i < 4; ++i)
		str[20+i] = tmp32[i];

	convert_to_char64(StreamExtensionEntry.dataLength, tmp64);
	for (int i = 0; i < 8; ++i)
		str[24+i] = tmp32[i];
}

streamExtensionEntry cts_streamExtensionEntry (unsigned char * const str)
{
	streamExtensionEntry StreamExtensionEntry;
	char tmp8;
	char tmp16[2];
	char tmp32[4];
	char tmp64[8];

	strncpy(&tmp8, str + 0, 1);
	StreamExtensionEntry.EntryType = convert_to_int8(&tmp8);

	strncpy(&tmp8, str + 1, 1);
	StreamExtensionEntry.generalSecondaryFlags = convert_to_int8(&tmp8);

	StreamExtensionEntry.nameLength = convert_to_int8(&str[3]);

	for (int i = 0; i < 2; i++)
	 	tmp16[0+i] = str[4+i];

	StreamExtensionEntry.nameHash = convert_to_int16(tmp16);	

	for (int i = 0; i < 8; i++)
	 	tmp64[0+i] = str[8+i];

	StreamExtensionEntry.vaildDataLength = convert_to_int64(tmp64);

	for (int i = 0; i < 4; i++)
	 	tmp32[0+i] = str[20+i];

	StreamExtensionEntry.firstCluster = convert_to_int32(tmp32);

	for (int i = 0; i < 8; i++)
	 	tmp64[0+i] = str[24+i];

	StreamExtensionEntry.dataLength = convert_to_int64(tmp64);	

	return StreamExtensionEntry;
}


void ctc_fileNameEntry(fileNameEntry FileNameEntry, unsigned char * const str)
{
	char tmp8;

	convert_to_char8(FileNameEntry.EntryType, &tmp8);
	str[0] = tmp8;

	convert_to_char8(FileNameEntry.generalSecondaryFlags, &tmp8);
	str[1] = tmp8;

	for (int i = 0; i < 30; i++)
		str[2+i] = FileNameEntry.fileName[i];
}

fileNameEntry cts_fileNameEntry(unsigned char * const str)
{
	fileNameEntry FileNameEntry;

	FileNameEntry.EntryType = convert_to_int8(&str[0]);
	FileNameEntry.generalSecondaryFlags = convert_to_int8(&str[1]);

	for (int i = 0; i < 30; i++)
		FileNameEntry.fileName[i] = str[2+i];

	return FileNameEntry;
}