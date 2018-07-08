#include "led.h"
#include "relay.h"

int main(void)
{
	int no = 0;
	int i = 0;
	
	led_init();
	relay_init();
	while (1) {
		led_on(no);
		relay_on(no);
		for (i = 0; i < 5000000; i++);
		led_off(no);
		relay_off(no);
		for (i = 0; i < 5000000; i++);
		no = (no + 1) % 3;
	}
}
