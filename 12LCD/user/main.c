#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"
#include "delay.h"
#include "eint.h"
#include "wdt.h"
#include "uart.h"
#include "lcd.h"

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
	
	LCD_DisplayChar(50, 50, c, 12);
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
	LCD_Init();
	
	//LCD_Fill_onecolor(0, 0, 100, 100, RED);
	//LCD_Fill_onecolor(50, 50, 150, 150, GREEN);
	//LCD_Draw_Line(50, 50, 100, 50);
	//LCD_Draw_Line(50, 50, 50, 150);
	//LCD_Draw_Line(50, 100, 100, 100);
	
	//LCD_DisplayChar(50, 100, 'A', 24);
	//LCD_DisplayNum(50, 200, 123, 4, 16, 0);
	LCD_DisplayChinese(50, 50, 0, 32);
	LCD_DisplayChinese(100, 50, 1, 32);
	LCD_DisplayChinese(150, 50, 2, 32);
	while (1) {
		uart_send(c);
		c++;
		if (c > 'Z') {
			c = 'A';
		}
		
		delay_ms(1000);
	}
}


