#include "led.h"

int main(void)
{
	int no = 0;
	int i = 0;
	
	led_init();
	while (1) {
		led_on(no);
		for (i = 0; i < 1000000; i++);
		led_off(no);
		for (i = 0; i < 1000000; i++);
		no = (no + 1) % 3;
	}
}
