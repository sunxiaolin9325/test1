#include "relay.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void relay_init(void)
{
	GPIO_InitTypeDef initValue = {0};
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOG, ENABLE);
	
	initValue.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	initValue.GPIO_Mode = GPIO_Mode_OUT;
	initValue.GPIO_OType = GPIO_OType_PP;
	initValue.GPIO_Speed = GPIO_Speed_50MHz;
	initValue.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &initValue);
	
	initValue.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOG, &initValue);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_ResetBits(GPIOG, GPIO_Pin_7);
}

void relay_off(int no)
{
	switch (no) {
		case 0:
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);
			break;
		case 1:
			GPIO_ResetBits(GPIOB, GPIO_Pin_7);
			break;
		case 2:
			GPIO_ResetBits(GPIOG, GPIO_Pin_7);
			break;
		default:
			break;
	}
}

void relay_on(int no)
{
	switch (no) {
		case 0:
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
			break;
		case 1:
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
			break;
		case 2:
			GPIO_SetBits(GPIOG, GPIO_Pin_7);
			break;
		default:
			break;
	}
}

