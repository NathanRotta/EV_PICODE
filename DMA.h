


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define DMA_CHANNEL0_BASE  0x7000


// Location of peripheral registers in physical memory
//#define PHYS_REG_BASE  0x20000000  // Pi Zero or 1
//#define PHYS_REG_BASE    0x3F000000  // Pi 2 or 3
#define PHYS_REG_BASE  0xFE000000  // Pi 4

// Location of peripheral registers in bus memory
#define BUS_REG_BASE    0x7E000000

// If non-zero, print debug information
#define DEBUG           1


// Convert memory bus address to physical address (for mmap)
#define BUS_PHYS_ADDR(a) ((void *)((uint32_t)(a)&~0xC0000000))

// Videocore mailbox memory allocation flags, see:
//     https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
typedef enum {
    MEM_FLAG_DISCARDABLE    = 1<<0, // can be resized to 0 at any time. Use for cached data
    MEM_FLAG_NORMAL         = 0<<2, // normal allocating alias. Don't use from ARM
    MEM_FLAG_DIRECT         = 1<<2, // 0xC alias uncached
    MEM_FLAG_COHERENT       = 2<<2, // 0x8 alias. Non-allocating in L2 but coherent
    MEM_FLAG_ZERO           = 1<<4, // initialise buffer to all zeros
    MEM_FLAG_NO_INIT        = 1<<5, // don't initialise (default is initialise to all ones)
    MEM_FLAG_HINT_PERMALOCK = 1<<6, // Likely to be locked for long periods of time
    MEM_FLAG_L1_NONALLOCATING=(MEM_FLAG_DIRECT | MEM_FLAG_COHERENT) // Allocating in L2
} VC_ALLOC_FLAGS;

// VC flags for unchached DMA memory
#define DMA_MEM_FLAGS (MEM_FLAG_DIRECT|MEM_FLAG_ZERO)

// Mailbox command/response structure
typedef struct {
    uint32_t len,   // Overall length (bytes)
        req,        // Zero for request, 1<<31 for response
        tag,        // Command number
        blen,       // Buffer length (bytes)
        dlen;       // Data length (bytes)
        uint32_t uints[32-5];   // Data (108 bytes maximum)
} VC_MSG __attribute__ ((aligned (16)));

// DMA register definitions
#define DMA_CHAN        5
#define DMA_PWM_DREQ    5
#define DMA_BASE        (PHYS_REG_BASE + 0x007000)
#define DMA_CS          (DMA_CHAN*0x100)
#define DMA_CONBLK_AD   (DMA_CHAN*0x100 + 0x04)
#define DMA_TI          (DMA_CHAN*0x100 + 0x08)
#define DMA_SRCE_AD     (DMA_CHAN*0x100 + 0x0c)
#define DMA_DEST_AD     (DMA_CHAN*0x100 + 0x10)
#define DMA_TXFR_LEN    (DMA_CHAN*0x100 + 0x14)
#define DMA_STRIDE      (DMA_CHAN*0x100 + 0x18)
#define DMA_NEXTCONBK   (DMA_CHAN*0x100 + 0x1c)
#define DMA_DEBUG       (DMA_CHAN*0x100 + 0x20)
#define DMA_ENABLE      0xff0
#define VIRT_DMA_REG(a) ((volatile uint32_t *)((uint32_t)virt_dma_regs + a))

char *dma_regstrs[] = {"DMA CS", "CB_AD", "TI", "SRCE_AD", "DEST_AD",
    "TFR_LEN", "STRIDE", "NEXT_CB", "DEBUG", ""};
    
#define DMA_CB_DEST_INC (1<<4)
#define DMA_CB_SRC_INC  (1<<8)

// Virtual memory for DMA descriptors and data buffers (uncached)
void *virt_dma_mem;

    
// DMA control block (must be 32-byte aligned)
typedef struct {
    uint32_t ti,    // Transfer info
        srce_ad,    // Source address
        dest_ad,    // Destination address
        tfr_len,    // Transfer length
        stride,     // Transfer stride
        next_cb,    // Next control block
        debug,      // Debug register, zero in control block
        unused;
} DMA_CB __attribute__ ((aligned(32)));


// Convert virtual DMA data address to a bus address
#define BUS_DMA_MEM(a)  ((uint32_t)a-(uint32_t)virt_dma_mem+(uint32_t)bus_dma_mem)
// PWM controller
#define PWM_BASE        (PHYS_REG_BASE + 0x20C000)
#define PWM_CTL         0x00   // Control
#define PWM_STA         0x04   // Status
#define PWM_DMAC        0x08   // DMA control
#define PWM_RNG1        0x10   // Channel 1 range
#define PWM_DAT1        0x14   // Channel 1 data
#define PWM_FIF1        0x18   // Channel 1 fifo
#define PWM_RNG2        0x20   // Channel 2 range
#define PWM_DAT2        0x24   // Channel 2 data
#define VIRT_PWM_REG(a) ((volatile uint32_t *)((uint32_t)virt_pwm_regs + (a)))
#define BUS_PWM_REG(a)  (PWM_BASE-PHYS_REG_BASE+BUS_REG_BASE+(uint32_t)(a))
#define PWM_CTL_RPTL1   (1<<2)  // Chan 1: repeat last data when FIFO empty
#define PWM_CTL_USEF1   (1<<5)  // Chan 1: use FIFO
#define PWM_DMAC_ENAB   (1<<31) // Start PWM DMA
#define PWM_ENAB        1       // Enable PWM
#define PWM_PIN         18      // GPIO pin for PWM output
#define PWM_OUT         1       // Set non-zero to enable PWM output pin

// Clock
void *virt_clk_regs;
#define CLK_BASE        (PHYS_REG_BASE + 0x101000)
#define CLK_PWM_CTL     0xa0
#define CLK_PWM_DIV     0xa4
#define VIRT_CLK_REG(a) ((volatile uint32_t *)((uint32_t)virt_clk_regs + (a)))
#define CLK_PASSWD      0x5a000000
#define CLOCK_KHZ       250000
#define PWM_CLOCK_ID    0xa

#define FAIL(x) {printf(x); terminate(0);}




class DMA{
	public:
	DMA();
	~DMA();
	private:
	
	
	bcm2711_peripheral DMAPeri;
	
	// VC mailbox file descriptor & handle, and bus memory pointer
	int mbox_fd, dma_mem_h;
	void *bus_dma_mem;
}





class MailBox{
	
	public
	MailBox();
	~MailBox();
	uint32_t msg_mbox();
	VC_MSG msgp;
	
	
	
	
	private:
	int fd;
	
	
}





