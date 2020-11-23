#include "gpio.h"
#include <stdio.h>

int main()
{
	IOPins pins;
	pins.func_sel(4, DIR_OUTPUT);
	while(1)
	{
		// Toggle pin 7 (blink a led!)
		pins.pin_set(4);
		sleep(1);
	 
		pins.pin_clear(4);
		sleep(1);
	}
 
  return 0; 
}
