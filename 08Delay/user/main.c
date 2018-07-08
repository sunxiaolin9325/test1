#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"
#include "delay.h"

int main(void)
{											
	led_init();
	relay_init();
	buzzer_init();
	delay_init();
	
	while (1) {
		led_on(0);
		relay_on(0);
		buzzer_on();
		delay_ms(500);
		led_off(0);
		relay_off(0);
		buzzer_off();
		delay_ms(500);
	}
}


