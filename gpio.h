

#ifndef GPIO
#define GPIO

#include "memory_map.h"




#define GPIO_BASE               (0x200000+BCM2711_PERI_BASE)// GPIO controlller 
#define DIR_INPUT 0
#define DIR_OUTPUT 1

class IOPins{
	//function prototypes
	public:
	void pin_clear(unsigned int pinnum);
	void  pin_set(unsigned int pinnum);
	//void pin_dir(unsigned int pin, unsigned char dir);
	IOPins();
	~IOPins();
	void func_sel(unsigned int pin,unsigned int funcode);
	private:
	struct bcm2711_peripheral gpio;

};

/*
Directly copied from GPIO section of https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2711/rpi_DATA_2711_1p0.pdf
These memory locations are the offsets from gpio base that map to the registers
*/
#define GPFSEL0_OFFSET 			0x00//GPIO Function Select
#define GPFSEL1_OFFSET          0x04//GPIO Function Select 
#define GPFSEL2_OFFSET          0x08//GPIO Function Select
#define GPFSEL3_OFFSET          0x0C//GPIO Function Select 
#define GPFSEL4_OFFSET          0x10//GPIO Function Select 
#define GPFSEL5_OFFSET          0x14//GPIO Function Select 
#define GPSET0_OFFSET           0x1C//GPIO Pin Output Set 
#define GPSET1_OFFSET           0x20//GPIO Pin Output Set 
#define GPCLR0_OFFSET           0x28//GPIO Pin Output Clear
#define GPCLR1_OFFSET           0x2C//GPIO Pin Output Clear
#define GPLEV0_OFFSET           0x34//GPIO Pin Level 0
#define GPLEV1_OFFSET           0x38//GPIO Pin Level 1
#define GPEDS0_OFFSET           0x40//GPIO Pin Event Detect Status 
#define GPEDS1_OFFSET           0x44//GPIO Pin Event Detect Status 
#define GPREN0_OFFSET           0x4C//GPIO Pin Rising Edge Detect Enable 
#define GPREN1_OFFSET           0x50//GPIO Pin Rising Edge Detect Enable
#define GPFEN0_OFFSET           0x58//GPIO Pin Falling Edge Detect Enable 
#define GPFEN1_OFFSET           0x5C//GPIO Pin Falling Edge Detect Enable 
#define GPHEN0_OFFSET           0x64//GPIO Pin High Detect Enable 0 32
#define GPHEN1_OFFSET           0x68//GPIO Pin High Detect Enable 1 32
#define GPLEN0_OFFSET           0x70//GPIO Pin Low Detect Enable 0 32
#define GPLEN1_OFFSET           0x74//GPIO Pin Low Detect Enable 1 32
#define GPAREN0_OFFSET          0x7C//GPIO Pin Async. Rising Edge Detect 
#define GPAREN1_OFFSET          0x80//GPIO Pin Async. Rising Edge Detect 
#define GPAFEN0_OFFSET          0x88//GPIO Pin Async. Falling Edge Detect
#define GPAFEN1_OFFSET          0x8C//GPIO Pin Async. Falling Edge Detect
#define GPIO_PUP_PDN_CNTRL_REG0_OFFSET 0xE4//GPIO Pull-up / Pull-down Register 
#define GPIO_PUP_PDN_CNTRL_REG1_OFFSET 0xE8//GPIO Pull-up / Pull-down Register 
#define GPIO_PUP_PDN_CNTRL_REG2_OFFSET 0xEC//GPIO Pull-up / Pull-down Register 
#define GPIO_PUP_PDN_CNTRL_REG3_OFFSET 0xF0//GPIO Pull-up / Pull-down Register 


/*
actual mapped addresses to the following registers on the deviece. the bitshift is to account for the fact that 
we are addressing by the 32 bit word length but the addresses in the datasheet are byte addressed.
*/
#define GPFSEL0    *(gpio.addr+(GPFSEL0_OFFSET>>2))
#define GPFSEL1    *(gpio.addr+(GPFSEL1_OFFSET>>2))
#define GPFSEL2    *(gpio.addr+(GPFSEL2_OFFSET>>2))
#define GPFSEL3    *(gpio.addr+(GPFSEL3_OFFSET>>2))
#define GPFSEL4    *(gpio.addr+(GPFSEL4_OFFSET>>2))
#define GPFSEL5    *(gpio.addr+(GPFSEL5_OFFSET>>2))
#define GPSET0     *(gpio.addr+(GPSET0_OFFSET>>2)) 
#define GPSET1     *(gpio.addr+(GPSET1_OFFSET>>2)) 
#define GPCLR0     *(gpio.addr+(GPCLR0_OFFSET>>2)) 
#define GPCLR1     *(gpio.addr+(GPCLR1_OFFSET>>2)) 
#define GPLEV0     *(gpio.addr+(GPLEV0_OFFSET>>2)) 
#define GPLEV1     *(gpio.addr+(GPLEV1_OFFSET>>2)) 
#define GPEDS0     *(gpio.addr+(GPEDS0_OFFSET>>2)) 
#define GPEDS1     *(gpio.addr+(GPEDS1_OFFSET>>2)) 
#define GPREN0     *(gpio.addr+(GPREN0_OFFSET>>2)) 
#define GPREN1     *(gpio.addr+(GPREN1_OFFSET>>2)) 
#define GPFEN0     *(gpio.addr+(GPFEN0_OFFSET>>2)) 
#define GPFEN1     *(gpio.addr+(GPFEN1_OFFSET>>2)) 
#define GPHEN0     *(gpio.addr+(GPHEN0_OFFSET>>2)) 
#define GPHEN1     *(gpio.addr+(GPHEN1_OFFSET>>2)) 
#define GPLEN0     *(gpio.addr+(GPLEN0_OFFSET>>2)) 
#define GPLEN1     *(gpio.addr+(GPLEN1_OFFSET>>2)) 
#define GPAREN0    *(gpio.addr+(GPAREN0_OFFSET>>2))
#define GPAREN1    *(gpio.addr+(GPAREN1_OFFSET>>2))
#define GPAFEN0    *(gpio.addr+(GPAFEN0_OFFSET>>2))
#define GPAFEN1    *(gpio.addr+(GPAFEN1_OFFSET>>2))
#define GPIO_PUP_PDN_CNTRL_REG0  *(gpio.addr + (GPIO_PUP_PDN_CNTRL_REG0>>2))
#define GPIO_PUP_PDN_CNTRL_REG1  *(gpio.addr + (GPIO_PUP_PDN_CNTRL_REG1>>2))
#define GPIO_PUP_PDN_CNTRL_REG2  *(gpio.addr + (GPIO_PUP_PDN_CNTRL_REG2>>2))
#define GPIO_PUP_PDN_CNTRL_REG3  *(gpio.addr + (GPIO_PUP_PDN_CNTRL_REG3>>2))



















#define INP_GPIO(g)   *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))//deprecated use func_sel
#define OUT_GPIO(g)   *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))


#endif
