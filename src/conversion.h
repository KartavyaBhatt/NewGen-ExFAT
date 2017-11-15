#ifndef CONVERSION_H
#define  CONVERSION_H

#include "fs.h"

void convert_to_char8(uint8_t z, unsigned char * const ser);
uint8_t convert_to_int8(unsigned char * const ser);

void convert_to_char16(uint16_t z, unsigned char * const ser);
uint16_t convert_to_int16(unsigned char * const ser);

void convert_to_char32(uint32_t z, unsigned char * const ser);
uint32_t convert_to_int32(unsigned char * const ser);

void convert_to_char64(uint64_t z, unsigned char * const ser);
uint64_t convert_to_int64(unsigned char * const ser);

void ctc_allocationBMapEntry(allocationBMapEntry AllocationBMapEntry, unsigned char * const str);
allocationBMapEntry cts_allocationBMapEntry(unsigned char * const str);




#endif