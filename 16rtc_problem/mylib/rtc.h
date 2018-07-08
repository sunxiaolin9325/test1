#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f4xx.h"

struct time_st {
	uint32_t Y;
	uint32_t M;
	uint32_t D;
	uint32_t h;
	uint32_t m;
	uint32_t s;
	uint32_t w;
};

extern int rtc_init(struct time_st *t);
extern void rtc_get_time(struct time_st *t);
extern void rtc_set_time(struct time_st *t);

#endif
