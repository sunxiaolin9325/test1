#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"

int main(void)
{
	int no = 0;
	int state;
		
	led_init();
	relay_init();
	button_init();
	buzzer_init();
	while (1) {
		state = button_state(no);
		if (no == 3) {
			if (state) {
				buzzer_on();
			} else {
				buzzer_off();
			}
		} else {
			if (state) {
				led_on(no);
				relay_on(no);
			} else {
				led_off(no);
				relay_off(no);
			}
		}
		
		no = (no + 1) % 4;
	}
}
