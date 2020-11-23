

#include "gpio.h"
#include "memory_map.h"


//struct bcm2711_peripheral gpio = {GPIO_BASE};
unsigned char is_initialized = 0;


//Constuctor
IOPins::IOPins()
{
	gpio.addr_p = GPIO_BASE;
	if(map_peripheral(&gpio) == -1) 
	  {
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		throw;
	  }
}
IOPins::~IOPins()
{
	unmap_peripheral(&gpio);
}

void IOPins::func_sel(unsigned int pin,unsigned int funcode)//input function code is zero output is 1
{
	
	if(funcode>7)
	{
		printf("invalide function code\n");
		return;
	}
	unsigned int bitshift= ((pin)%10)*3;//as it happens the function pins are in groups three changing registers every 10
	unsigned int clrpins=(~(funcode<<bitshift))^(7<<bitshift);//gets the function pins that need to be cleared
	unsigned int setpins=funcode<<bitshift;//gets pins that need to be set
	volatile unsigned int * addr = gpio.addr+pin/10;//gets the address of the funciton register
	
	//printf("The clrpins are:%X\nThe setpins are:%X\n",clrpins,setpins);
	
	*addr &=clrpins;
	*addr|=setpins;
	//printf("GPFSEL0 is %X \n",GPFSEL0);
	//printf("GPFSEL2 is %X \n",GPFSEL0);
}

/*Deprecated use func_sel

void IOPins::pin_dir(unsigned int pin, unsigned char dir)//sets the pin direction to either input or output
{
	if(dir==DIR_INPUT)
	{
		INP_GPIO(pin);
	}
	else if(dir==DIR_OUTPUT)
	{
		OUT_GPIO(pin);
	}
	else
	{
		printf("Invalid input into pin_dir");
	}
}*/

void  IOPins::pin_set(unsigned int pinnum)
{
	GPSET0 = 1 << pinnum;
}

void IOPins::pin_clear(unsigned int pinnum)
{
	GPCLR0 = 1 << pinnum;
}






