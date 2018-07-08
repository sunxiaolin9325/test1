#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"
//#define GPIOE_BASE		(0x40021000)
#define GPIOE_MODER		(*(volatile unsigned int *)(GPIOE_BASE	+ 0x00))
#define GPIOG_MODER		(*(volatile unsigned int *)(GPIOG_BASE	+ 0x00))
#define GPIOE_OTYPER	(*(volatile unsigned int *)(GPIOE_BASE	+ 0x04))
#define GPIOG_OTYPER	(*(volatile unsigned int *)(GPIOG_BASE	+ 0x04))
#define GPIOE_OSPEEDR	(*(volatile unsigned int *)(GPIOE_BASE	+ 0x08))
#define GPIOG_OSPEEDR	(*(volatile unsigned int *)(GPIOG_BASE	+ 0x08))
#define GPIOE_PUPDR		(*(volatile unsigned int *)(GPIOE_BASE	+ 0x0C))
#define GPIOG_PUPDR		(*(volatile unsigned int *)(GPIOG_BASE	+ 0x0C))
#define GPIOE_ODR			(*(volatile unsigned int *)(GPIOE_BASE	+ 0x14))
#define GPIOG_ODR			(*(volatile unsigned int *)(GPIOG_BASE	+ 0x14))


extern void led_init(void);
extern void led_off(int no);
extern void led_on(int no);

#endif //__LED_H__

