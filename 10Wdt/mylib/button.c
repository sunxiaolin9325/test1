#include "button.h"
#include "stm32f4xx_conf.h"
#include "bitband.h"

void button_init(void)
{
	GPIO_InitTypeDef initValue = {0};
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	
	initValue.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7; 
	initValue.GPIO_Mode = GPIO_Mode_IN;
	initValue.GPIO_PuPd = GPIO_PuPd_NOPULL;
 	GPIO_Init(GPIOF, &initValue);
	
	initValue.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &initValue);
}

int button_state(int no)
{
	int ret = 0;
	
	switch (no) {
		case 0:
			//ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9);
			ret = PFIn(9);
			break;
		case 1:
			//ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8);
			ret = PFIn(8);
			break;
		case 2:
			//ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7);
			ret = PFIn(7);
			break;
		case 3:
			//ret = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);
			ret = PEIn(6);
			break;
		default:
			ret = 1;
			break;
	}
	
	return !ret;
}





