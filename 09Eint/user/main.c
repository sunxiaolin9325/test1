#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"
#include "delay.h"
#include "eint.h"

void f1(void)
{
	led_on(0);
	led_on(1);
	led_on(2);
	relay_on(0);
	relay_on(1);
	relay_on(2);
}

void f2(void)
{
	led_off(0);
	led_off(1);
	led_off(2);
	relay_off(0);
	relay_off(1);
	relay_off(2);
}

void f3(void)
{
	buzzer_on();
}

void f4(void)
{
	buzzer_off();
}

int main(void)
{											
	// 分配中断控制器优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	relay_init();
	buzzer_init();
	delay_init();
	eint_init();
	set_handler(f1, f2, f3, f4);
	
	while (1);
}


