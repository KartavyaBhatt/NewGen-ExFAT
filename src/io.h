#ifndef IO_H 
#define IO_H

#include <stdbool.h>
#include <inttypes.h>

bool write_cluster(uint8_t *data, long int clusterNumber);
bool read_cluster(uint8_t *buff, long int clusterNumber);
uint32_t get_freeCluster();
void set_cluster_bitmap1(uint32_t clusterNumber);
void set_cluster_bitmap2(uint32_t clusterNumber);
static bool rootDir_init();

#endif