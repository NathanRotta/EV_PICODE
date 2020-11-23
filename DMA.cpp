#include "memory_map.h"

#include "DMA.h"

#include <stdio.h>
#include <string.h>

#define MMAP_FLAGS (MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE|MAP_LOCKED)

DMA::DMA()
{
	DMAPeri=BCM2711_PERI_BASE+DMA_CHANNEL0_BASE;
	if(-1==map_peripheral(&DMAPeri))
	{
		printf("Failed to map DMA peripheral");
		throw;
	}
	//void * mem = mmap(0, BLOCK_SIZE, MMAP_FLAGS, DMAPeri.mem_fd, 0);
	
}
DMA::~DMA()
{
	unmap_peripheral(&DMAPeri);
}








// Open mailbox interface, return file descriptor
int open_mbox(void)
{
   int fd;
 
   if ((fd = open("/dev/vcio", 0)) < 0)
       FAIL("Error: can't open VC mailbox\n");
   return(fd);
}
// Send message to mailbox, return first response int, 0 if error
uint32_t msg_mbox(int fd, VC_MSG *msgp)
{
    uint32_t ret=0, i;
 
    for (i=msgp->dlen/4; i<=msgp->blen/4; i+=4)
        msgp->uints[i++] = 0;
    msgp->len = (msgp->blen + 6) * 4;
    msgp->req = 0;
    if (ioctl(fd, _IOWR(100, 0, void *), msgp) < 0)
        printf("VC IOCTL failed\n");
    else if ((msgp->req&0x80000000) == 0)
        printf("VC IOCTL error\n");
    else if (msgp->req == 0x80000001)
        printf("VC IOCTL partial error\n");
    else
        ret = msgp->uints[0];
    return(ret);
}
// Allocate memory on PAGE_SIZE boundary, return handle
uint32_t alloc_vc_mem(int fd, uint32_t size, VC_ALLOC_FLAGS flags)
{
    VC_MSG msg={.tag=0x3000c, .blen=12, .dlen=12,
        .uints={PAGE_ROUNDUP(size), PAGE_SIZE, flags}};
    return(msg_mbox(fd, &msg));
}
// Lock allocated memory, return bus address
void *lock_vc_mem(int fd, int h)
{
    VC_MSG msg={.tag=0x3000d, .blen=4, .dlen=4, .uints={h}};
    return(h ? (void *)msg_mbox(fd, &msg) : 0);
}