#include "rtc.h"

int rtc_init(struct time_st *t)
{
	uint32_t backupRegister;
	RTC_InitTypeDef RTC_InitValue;
	
	//1.打开pwr时钟，因为要利用pwr来控制VBAT(备份电路，其中包括RTC和RTC的备份寄存器)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//2.关闭RTC的写保护，默认是写保护的(只能读不能写)
	PWR_BackupAccessCmd(ENABLE);
	//3.设定时间（先读取备份寄存器的值来判断是否是第一次运行rtc_init函数)
	//注意：backupRegister有很多个，下面这个函数每次读取4个字，RTC_BKP_DR1寄存器的编号
	backupRegister = RTC_ReadBackupRegister(RTC_BKP_DR1);
	if (backupRegister != 0x0101) {
	//4.如果是第一次运行rtc_init函数则：
	//		a.打开LSE时钟（32768HZ）
			RCC_LSEConfig(RCC_LSE_ON);
	//		b.等待LSE稳定'
			while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	//		c.为RTC选择时钟源
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	//		d.是能RTC的时钟
			RCC_RTCCLKCmd(ENABLE);
	//		e.初始化RTC(RTC_Init)
			RTC_InitValue.RTC_AsynchPrediv = 127;
			RTC_InitValue.RTC_HourFormat = RTC_HourFormat_24;
			RTC_InitValue.RTC_SynchPrediv = 255;
			RTC_Init(&RTC_InitValue);
	//		f.设置时间和日期
			rtc_set_time(t);
	//		g.写备份寄存器（作为已经设置过时间的标志）
			RTC_WriteBackupRegister(RTC_BKP_DR1, 0x0101);
	}
	//5.打开RTC的写保护
	PWR_BackupAccessCmd(DISABLE);
	return  backupRegister;
}

void rtc_get_time(struct time_st *t)
{
	//获得时间
	RTC_TimeTypeDef RTC_TimeValue;
	RTC_DateTypeDef RTC_DateValue;
	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeValue);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateValue);
	
	t->Y = RTC_DateValue.RTC_Year + 1900;
	t->M = RTC_DateValue.RTC_Month + 1;
	t->D = RTC_DateValue.RTC_Date;
	t->w = RTC_DateValue.RTC_WeekDay + 1;
	
	t->h = RTC_TimeValue.RTC_Hours;
	t->m = RTC_TimeValue.RTC_Minutes;
	t->s = RTC_TimeValue.RTC_Seconds;
}

//调用此函数之前要关闭VBAT的写保护
void rtc_set_time(struct time_st *t)
{
	RTC_TimeTypeDef RTC_TimeValue;
	RTC_DateTypeDef RTC_DateValue;
	
	//设置时间
	//RTC_TimeValue.RTC_H12 = t->am;
	RTC_TimeValue.RTC_Hours = t->h;
	RTC_TimeValue.RTC_Minutes = t->m;
	RTC_TimeValue.RTC_Seconds = t->s;
	
	RTC_DateValue.RTC_Date = t->D;
	RTC_DateValue.RTC_Month = t->M - 1;
	RTC_DateValue.RTC_WeekDay = t->w - 1;
	RTC_DateValue.RTC_Year = t->Y - 1900;
	
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeValue);
	RTC_SetDate(RTC_Format_BIN, &RTC_DateValue);
}



