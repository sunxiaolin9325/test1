/*
*/

#include "gpio_iic.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "bitband.h"

/******************硬件相关*****************/
GPIO_InitTypeDef GPIO_InitForSCLOut;
GPIO_InitTypeDef GPIO_InitForSDAOut;
GPIO_InitTypeDef GPIO_InitForSDAIn;

static void set_scl_val(int value)
{
	GPIO_Init(GPIOB, &GPIO_InitForSCLOut);
	PBOut(8) = value;
}

static void set_sda_value(int value)
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAOut);
	PBOut(9) = value;
}

static uint8_t get_sda_value()
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAIn);
	return PBIn(9);
}

// gpio_iic初始化，都拉高
void IIC_Init(void)
{			
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitForSCLOut.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitForSCLOut.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitForSCLOut.GPIO_OType = GPIO_OType_PP;
	GPIO_InitForSCLOut.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitForSCLOut.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitForSDAOut.GPIO_Pin = GPIO_Pin_9;	// 连接到EEPROM(IIC设备)的SDA
	GPIO_InitForSDAOut.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitForSDAOut.GPIO_OType = GPIO_OType_PP;
	GPIO_InitForSDAOut.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitForSDAOut.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_InitForSDAIn.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitForSDAIn.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitForSDAIn.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitForSDAIn.GPIO_PuPd = GPIO_PuPd_UP;
	
	set_scl_val(1);
	set_sda_value(1);
}

/************硬件无关**********************/
//产生开始信号
void IIC_Start(void)
{			
	set_scl_val(1);
	set_sda_value(1);
	delay_us(2);
	set_sda_value(0);
	delay_us(2);
	set_scl_val(0);
}	  

//产生IIC停止信号
void IIC_Stop(void)
{
	set_scl_val(0);
	set_sda_value(0);
 	delay_us(2);
	set_scl_val(1);
	delay_us(1);
	set_sda_value(1);
	delay_us(2);							   	
}

//等待应答信号到来
uint8_t IIC_Wait_Ack(void)
{
	int timeout = 0; 

	//set_sda_value(1);
	set_scl_val(0);
	delay_us(1);
	set_scl_val(1);
	delay_us(1);
	while (get_sda_value()) {
		if (timeout > 250) {
			IIC_Stop();
			return 1;
		}
		timeout++;
	}
	set_scl_val(0);
	return 0;  
} 

//产生ACK
void IIC_Ack(void)
{
	set_scl_val(0);
	set_sda_value(0);
	delay_us(2);
	set_scl_val(1);
	delay_us(2);
	set_scl_val(0);
}

//不产生ACK    
void IIC_NAck(void)
{
	set_scl_val(0);
	set_sda_value(1);
	delay_us(2);
	set_scl_val(1);
	delay_us(2);
	set_scl_val(0);
}	

//IIC发送一个字节
void IIC_Send_Byte(uint8_t data)
{                        
	int i;
  
	for (i = 0; i < 8; i++) {  
		set_scl_val(0);
		delay_us(1);
		set_sda_value(data >> 7);
		data <<= 1; 	  
		set_scl_val(1);
		delay_us(2); 
	}
	set_scl_val(0);
} 

//IIC接收1个字节
uint8_t IIC_Recv_Byte(uint8_t ack)
{
	int i;
	uint8_t receive = 0;
	
	for (i=0; i<8; i++) {
		set_scl_val(0);
		delay_us(2);
		set_scl_val(1);
		delay_us(2);
		receive <<= 1;
		if (get_sda_value()) {
			receive |= 1;   
		}
	}
	set_scl_val(0);
	
	if (!ack)
		IIC_NAck();
	else
		IIC_Ack(); 
	
	return receive;
}
