#include "delay.h"

static u8 fac_us = 0;
static u16 fac_ms = 0;

void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8;
	fac_ms = (u16) (fac_us * 1000);
}

void delay_us(u32 nus)
{
	u32 value;
	SysTick->LOAD = nus * fac_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do {
		value = SysTick->CTRL;
	} while ((value & 0x01) && !(value & (1 << 16)));
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
	SysTick->VAL = 0x00;
}

// Max 541
static void delay_xms(u16 nms)
{
	u32 value;
	SysTick->LOAD = (u32) (nms * fac_ms);
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do {
		value = SysTick->CTRL;
	} while ((value & 0x01) && !(value & (1 << 16)));
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
	SysTick->VAL = 0x00;
}

void delay_ms(u16 nms)
{
	u8 repeat = nms / 540;
	u16 remain = nms % 540;
	
	while (repeat) {
		delay_xms(540);
		repeat--;
	}
	
	if (remain) {
		delay_xms(remain);
	}
}
