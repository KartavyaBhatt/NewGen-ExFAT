#include <stdio.h>
#include <unistd.h>
#include "partition.c"
//#include "io.c"
#include "fs_io.c"

void main()
{
	uint8_t buff1[CLUSTERSIZE];
	uint8_t buff[32];
	uint8_t str[32];
	
	NGEF_init("FS", 7340032);                //Creating partiton of 1GB = 1073741824 bytes

	allocationBMapEntry AllocationBMapEntry1, AllocationBMapEntry2;


	AllocationBMapEntry1.EntryType = 0x81;
	AllocationBMapEntry1.bitmapFlags = 0x04;
	AllocationBMapEntry1.firstCluster = 0x02;
	AllocationBMapEntry1.dataLength = 0x02;

	cti_allocationBMapEntry(AllocationBMapEntry1, str);

	if(write_cluster(str, 3))
	{
		read_cluster(buff, 3);

		if (buff)
		{
			AllocationBMapEntry2 = cts_allocationBMapEntry(buff);
			printf("EntryType : %" PRIu8 "\n", AllocationBMapEntry2.EntryType);
			printf("bitmapFlags : %" PRIu8 "\n", AllocationBMapEntry2.bitmapFlags);
			printf("firstCluster : %" PRIu32 "\n", AllocationBMapEntry2.firstCluster);
			printf("dataLength : %" PRIu64 "\n", AllocationBMapEntry2.dataLength);
		}
	}
}