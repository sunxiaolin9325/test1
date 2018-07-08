#ifndef __USART4_H__
#define __USART4_H__

#include "stm32f4xx.h"

extern void usart4_send_data(char *buf);
extern void usart4_init(void);
extern void usart4_send(uint8_t c);
extern void usart4_recv_ok(char *buf);
extern void clear_recv_flag(void);
extern void usart4_recv_link_info(char *buf);

#endif
