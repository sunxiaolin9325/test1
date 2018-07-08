#include "buzzer.h"
#include "stm32f4xx_conf.h"

void buzzer_init(void)
{
	GPIO_InitTypeDef initValue = {0};
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	initValue.GPIO_Pin = GPIO_Pin_7;
	initValue.GPIO_Mode = GPIO_Mode_OUT;
	initValue.GPIO_OType = GPIO_OType_PP;
	initValue.GPIO_Speed = GPIO_Speed_50MHz;
	initValue.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &initValue);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}

void buzzer_off(void)
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}

void buzzer_on(void)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
}

