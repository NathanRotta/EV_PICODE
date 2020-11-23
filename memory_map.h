
#ifndef	MEMORY_MAP
#define MEMORY_MAP
//For details on the SoC see the following datasheet https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2711/rpi_DATA_2711_1p0.pdf
//I base much of this off this tutorial https://www.pieter-jan.com/node/15 adjusting for the differences in the bcm2711 SoC

#include <stdio.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#include <unistd.h>
 
#define BCM2711_PERI_BASE       0xFE000000 //This is specified as the base for accessing from ARM by the datasheet


/*
This is a different offset than one would think fromt the datasheet but there is a mistake https://github.com/raspberrypi/documentation/issues/1569
*/

 
#define BLOCK_SIZE 		(4*1024)
 
// IO Acces
struct bcm2711_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

int map_peripheral(struct bcm2711_peripheral *p);

void unmap_peripheral(struct bcm2711_peripheral *p);


#endif
