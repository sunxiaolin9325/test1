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
#include "pm25.h"
#include "rtc.h"
#include "eeprom.h"

#include <stdio.h>

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
	unsigned char c = 'A';
	char buf[16] = {0};
	char dht_buf[5] = {0};
	int ret = 123;
	int pm_value = 0;
	struct time_st tm = {2018, 02, 26, 23, 23, 00, 1};
	char time_buf[32] = {0};
	int i = 0;
	uint8_t eeprom_value = 0;
	
	// 分配中断控制器优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	led_init();
	relay_init();
	buzzer_init();
	delay_init();
	uart_init();
	setRecvHandler(usartRecvHandler);
	LCD_Init();
	DS18B20_Init();
	dht_init();
	pm_init();
	rtc_init(&tm);
	eeprom_init();
	
	LCD_DisplayChinese(50, 50, 0, 32);
	LCD_DisplayChinese(100, 50, 1, 32);
	LCD_DisplayChinese(150, 50, 2, 32);
		
	while (1) {
		eeprom_write_byte(0xA0, 10, (uint8_t)i);
		
		uart_send(c);
		c++;
		if (c > 'Z') {
			c = 'A';
		}
		
		get_temperature(buf);
		LCD_DisplayString(50, 100, 24, (u8 *)buf);
		
		ret = dht_get(dht_buf);
		if (!ret) {
			LCD_DisplayNum(50, 150, (u32)dht_buf[0], 3, 24, 0);
			LCD_DisplayNum(150, 150, (u32)dht_buf[2], 3, 24, 0);
		} else {
			LCD_DisplayString(50, 150, 24, (u8 *)"error");
		}
		
		pm_value = pm_get();
		LCD_DisplayNum(250, 150, pm_value, 5, 24, 0);
				
		rtc_get_time(&tm);
		sprintf(time_buf, "%4d-%02d-%02d %02d:%02d:%02d %d",
		tm.Y, tm.M, tm.D, tm.h, tm.m, tm.s, tm.w);
		LCD_DisplayString(0, 0, 24, (u8 *)time_buf);
		eeprom_value = eeprom_read_byte(0xA0, 10);
		LCD_DisplayNum(250, 50, eeprom_value, 3, 24, 0);
		delay_ms(1000);
	}
}


