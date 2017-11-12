#include <stdio.h>
#include <string.h>
#include "partition.h"

bool write_cluster(char *data, long int clusterNumber)
{
	mainBootRegion MainBootRegion = get_mainBootRegion();

	if (clusterNumber <= MainBootRegion.clusterCount && data != NULL)
	{
		FILE *volume = get_volume();
		cluster Cluster;
		

		strcpy(Cluster.data,data);

		// Debug :: printf("Data : %s\n", data);
		// Debug :: printf("Cluster data : %s\n", Cluster.data);

		fseek(volume, (MainBootRegion.clusterHeapOffset*SECTOR_SIZE) + (clusterNumber*CLUSTERSIZE), 0);
		
		if(fwrite(&Cluster, sizeof(Cluster), 1, volume) == 1)
			return true;
		else
			return false;
	}

	else
		return false;
}

bool read_cluster(char *buff, long int clusterNumber)
{
	mainBootRegion MainBootRegion = get_mainBootRegion();
	
	if (clusterNumber <= MainBootRegion.clusterCount && buff != NULL)
	{		
		FILE *volume = get_volume();
		cluster Cluster;

		fseek(volume, (MainBootRegion.clusterHeapOffset*SECTOR_SIZE) + (clusterNumber*CLUSTERSIZE), 0);
		
		if(fread(&Cluster, sizeof(Cluster), 1, volume) == 1)
		{
			// Debug :: printf("Read Cluster Data : %s\n", Cluster.data);
			strcpy(buff, Cluster.data);
			// Debug :: printf("Read Buff Data : %s\n", buff);
			return true;
		}
		else
			return false;
	}
}