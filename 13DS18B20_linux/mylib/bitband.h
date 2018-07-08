#ifndef __BITBAND_H__
#define __BITBAND_H__

#include <stm32f4xx.h>

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000	\
				+ ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) (*((volatile unsigned long *)addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
 
// GPIO input data register addr
#define GPIOA_IDR_ADDR		(GPIOA_BASE + 0x10)		// 0x40020010
#define GPIOB_IDR_ADDR		(GPIOB_BASE + 0x10)
#define GPIOC_IDR_ADDR		(GPIOC_BASE + 0x10)
#define GPIOD_IDR_ADDR		(GPIOD_BASE + 0x10)
#define GPIOE_IDR_ADDR		(GPIOE_BASE + 0x10)
#define GPIOF_IDR_ADDR		(GPIOF_BASE + 0x10)
#define GPIOG_IDR_ADDR		(GPIOG_BASE + 0x10)
#define GPIOH_IDR_ADDR		(GPIOH_BASE + 0x10)
// GPIO output data register addr
#define GPIOA_ODR_ADDR		(GPIOA_BASE + 0x14)		// 0x40020014
#define GPIOB_ODR_ADDR		(GPIOB_BASE + 0x14)
#define GPIOC_ODR_ADDR		(GPIOC_BASE + 0x14)
#define GPIOD_ODR_ADDR		(GPIOD_BASE + 0x14)
#define GPIOE_ODR_ADDR		(GPIOE_BASE + 0x14)
#define GPIOF_ODR_ADDR		(GPIOF_BASE + 0x14)
#define GPIOG_ODR_ADDR		(GPIOG_BASE + 0x14)
#define GPIOH_ODR_ADDR		(GPIOH_BASE + 0x14)

// input
#define PAIn(n)		BIT_ADDR(GPIOA_IDR_ADDR, n)
#define PBIn(n)		BIT_ADDR(GPIOB_IDR_ADDR, n)
#define PCIn(n)		BIT_ADDR(GPIOC_IDR_ADDR, n)
#define PDIn(n)		BIT_ADDR(GPIOD_IDR_ADDR, n)
#define PEIn(n)		BIT_ADDR(GPIOE_IDR_ADDR, n)
#define PFIn(n)		BIT_ADDR(GPIOF_IDR_ADDR, n)
#define PGIn(n)		BIT_ADDR(GPIOG_IDR_ADDR, n)
#define PHIn(n)		BIT_ADDR(GPIOH_IDR_ADDR, n)

// output
#define PAOut(n)		BIT_ADDR(GPIOA_ODR_ADDR, n)
#define PBOut(n)		BIT_ADDR(GPIOB_ODR_ADDR, n)
#define PCOut(n)		BIT_ADDR(GPIOC_ODR_ADDR, n)
#define PDOut(n)		BIT_ADDR(GPIOD_ODR_ADDR, n)
#define PEOut(n)		BIT_ADDR(GPIOE_ODR_ADDR, n)
#define PFOut(n)		BIT_ADDR(GPIOF_ODR_ADDR, n)
#define PGOut(n)		BIT_ADDR(GPIOG_ODR_ADDR, n)
#define PHOut(n)		BIT_ADDR(GPkIOH_ODR_ADDR, n)

#define PIn(group, n)	(P##groupIn(n))
#define POut(group, n)	(P##groupOut(n))

#endif

