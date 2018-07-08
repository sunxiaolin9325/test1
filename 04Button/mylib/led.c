#include "led.h"
#include "stm32f4xx_rcc.h"

void led_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIOE_MODER &= ~(0xf << 6);
	GPIOE_MODER |= (1 << 6) | (1 << 8);
	GPIOG_MODER &= ~(0x3 << 18);
	GPIOG_MODER |= (1 << 18);
	GPIOE_OTYPER &= ~(0x3 << 3);
	GPIOG_OTYPER &= ~(1 << 9);
	GPIOE_OSPEEDR &= ~(0xf << 6);
	GPIOE_OSPEEDR |= (1 << 7) | (1 << 9);
	GPIOG_OSPEEDR &= ~(0x3 << 18);
	GPIOG_OSPEEDR |= (1 << 19);
	GPIOE_PUPDR &= ~(0xf << 6);
	GPIOG_PUPDR &= ~(0x3 << 18);
	GPIOE_ODR |= (0x3 << 3);
 	GPIOG_ODR |= (1 << 9);
}

void led_off(int no)
{
	switch (no) {
		case 0:
			GPIOE_ODR |= (1 << 3);
			break;
		case 1:
			GPIOE_ODR |= (1 << 4);
			break;
		case 2:
			GPIOG_ODR |= (1 << 9);
			break;
		default:
			break;
	}
}

void led_on(int no)
{
	switch (no) {
		case 0:
			GPIOE_ODR &= ~(1 << 3);
			break;
		case 1:
			GPIOE_ODR &= ~(1 << 4);
			break;
		case 2:
			GPIOG_ODR &= ~(1 << 9);
			break;
		default:
			break;
	}
}
