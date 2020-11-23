#include "memory_map.h"


int map_peripheral(struct bcm2711_peripheral *p)
{
   // Open /dev/mem
   if ((p->mem_fd = open("/dev/mem", O_RDWR| O_SYNC) ) < 0) {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      return -1;
   }
 
   p->map = mmap(
      NULL,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p       // Address in physical map that we want this memory block to expose
   );
 
   if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
   }
 
   p->addr = (volatile unsigned int *)p->map;
 
   return 0;
}
// Return physical address of virtual memory
void *phys_mem(void *virt)
{
    uint64_t pageInfo;
    int file = open("/proc/self/pagemap", 'r');
     
    if (lseek(file, (((size_t)virt)/PAGE_SIZE)*8, SEEK_SET) != (size_t)virt>>9)
        printf("Error: can't find page map for %p\n", virt);
    read(file, &pageInfo, 8);
    close(file);
    return((void*)(size_t)((pageInfo*PAGE_SIZE)));
}
 
void unmap_peripheral(struct bcm2711_peripheral *p) {
 
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
}





