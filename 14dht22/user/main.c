#include "led.h"
#include "relay.h"
#include "button.h"
#include "buzzer.h"
#include "delay.h"
#include "eint.h"
#include "wdt.h"
#include "uart.h"
#include "lcd.h"
#include "ds18b20.h"
#include "dht.h"
#include "math.h"

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

// lnux yun
int main(void)
{											
//	unsigned char c = 'A';
	char buf[16] = {0};
	char dht_buf[5] = {0};
	int ret = 0;
	
	// 分配中断控制器优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	relay_init();
	buzzer_init();
	delay_init();
	//uart_init();
	setRecvHandler(usartRecvHandler);
	LCD_Init();
	DS18B20_Init();
	dht_init();
	
	LCD_DisplayChinese(50, 50, 0, 32);
	LCD_DisplayChinese(100, 50, 1, 32);
	LCD_DisplayChinese(150, 50, 2, 32);
	while (1) {
		/*
		uart_send(c);
		c++;
		if (c > 'Z') {
			c = 'A';
		}
		*/
		
		get_temperature(buf);
		LCD_DisplayString(50, 100, 24, (u8 *)buf);
		
		ret = dht_get(dht_buf);
		if (!ret) {
			LCD_DisplayNum(50, 150, (u32)dht_buf[0], 3, 24, 0);
			LCD_DisplayNum(150, 150, (u32)dht_buf[2], 3, 24, 0);
		} else if (ret == -1){
			LCD_DisplayString(50, 150, 24, (u8 *)"timeout");
		} else if (ret == -2) {
			LCD_DisplayString(50, 150, 24, (u8 *)"data errro");
		}
		
		delay_ms(1000);
	}
}


