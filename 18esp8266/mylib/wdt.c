#include "wdt.h"
#include "stm32f4xx_conf.h"

void wdt_init(int nms)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	// Max value: 4095
	IWDG_SetReload(nms / 8);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void feed_dog()
{
	IWDG_ReloadCounter();
}


