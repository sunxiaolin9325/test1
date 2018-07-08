/*
*/

#include "dht.h"
#include "stm32f4xx_conf.h"
#include "delay.h"
#include "bitband.h"

//GPIOB10
static void gpio_in()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void gpio_out()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void gpio_set_value(int value)
{
	gpio_out();
	if (value) {
		PBOut(10) = 1;
	} else {
		PBOut(10) = 0; 
	}
}

static int gpio_get_value()
{
	gpio_in();
	return PBIn(10);
}

void dht_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
}

// state 0: wait low	1: wait high
static int gpio_wait(int state, int timeout_us)
{
	while (gpio_get_value() != state && timeout_us > 0) {
		delay_us(2);
		timeout_us -= 2;
	}
	
	if (timeout_us <= 0) {
		return -1;
	}
	
	return 0;
}

int dht_get(char *buf)
{
	int i, j;
	
	gpio_set_value(1);
	delay_us(10);
	
	gpio_set_value(0);
	delay_ms(20);
	gpio_set_value(1);
	delay_us(30);
	if (gpio_wait(0, 100))
		return -1;
	if (gpio_wait(1, 100))
		return -1;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 8; j++) {
			if (gpio_wait(0, 100))
				return -1;
			if (gpio_wait(1, 100))
				return -1;
			delay_us(40);
			buf[i] <<= 1;
			buf[i] |= gpio_get_value();
		}
	}
	
	if (gpio_wait(0, 100))
		return -1;
	gpio_set_value(1);
	
	if (buf[0] + buf[1] + buf[2] + buf[3] != buf[4]) {
		return 0;
		//return -2;
	}
	
	return 0;
}

