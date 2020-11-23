#include "gpio.h"
#include <stdio.h>

int main()
{
	gpio_init();
	
	func_sel(4, DIR_OUTPUT);
	func_sel(20,DIR_INPUT);
	func_sel(20,DIR_OUTPUT);
	func_sel(4, DIR_INPUT);
	func_sel(4, DIR_OUTPUT);
	while(1)
	{
		// Toggle pin 7 (blink a led!)
		pin_set(4);
		sleep(1);
	 
		pin_clear(4);
		sleep(1);
	}
 
  return 0; 
}
