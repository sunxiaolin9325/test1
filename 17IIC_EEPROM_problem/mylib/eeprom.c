/*
*/

#include "eeprom.h"
#include "gpio_iic.h"
#include "delay.h"
#include "lcd.h"

void eeprom_init(void)
{
	IIC_Init();
}

void eeprom_write_byte(uint8_t daddr, uint8_t waddr, uint8_t data)
{
	IIC_Start();
	IIC_Send_Byte(daddr);
	if (IIC_Wait_Ack()) {
		LCD_Printf((uint8_t *)"a1======");
	}
	IIC_Send_Byte(waddr);
	if (IIC_Wait_Ack()) {
		LCD_Printf((uint8_t *)"a2======");
	}
	IIC_Send_Byte(data);
	if (IIC_Wait_Ack()) {
		LCD_Printf((uint8_t *)"a3======");
	}
	IIC_Stop();
	delay_ms(10);
}

uint8_t eeprom_read_byte(uint8_t daddr, uint8_t waddr)
{
	uint8_t value = 0;
	
	IIC_Start();
	IIC_Send_Byte(daddr);
	if (IIC_Wait_Ack()) {
		LCD_Printf((uint8_t *)"a4======");
		return 100;
	}
	IIC_Send_Byte(waddr);
	if (IIC_Wait_Ack()) {
		LCD_Printf((uint8_t *)"a5======");
		return 101;
	}
	IIC_Start();
	IIC_Send_Byte(daddr | 1);
	if (IIC_Wait_Ack()) {
		LCD_Printf((uint8_t *)"a6======");
		//return 102;
	}
	value = IIC_Recv_Byte(0);
	IIC_Stop();
	
	return value;
}


