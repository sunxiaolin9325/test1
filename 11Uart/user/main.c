#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"
#include "delay.h"
#include "eint.h"
#include "wdt.h"
#include "uart.h"

void usartRecvHandler(unsigned char c)
{
	switch (c) {
		case '0':
			buzzer_on();
			break;
		case '1':
			buzzer_off();
			break;
		default:
			led_on(0);
			led_on(1);
			led_on(2);
			break;
	}
}

int main(void)
{											
	unsigned char c = 'A';
	
	// 分配中断控制器优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	relay_init();
	buzzer_init();
	delay_init();
	uart_init();
	setRecvHandler(usartRecvHandler);
	
	while (1) {
		uart_send(c);
		c++;
		if (c > 'Z') {
			c = 'A';
		}
		
		delay_ms(1000);
	}
}


