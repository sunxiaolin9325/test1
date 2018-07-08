#ifndef __UART_H__
#define __UART_H_

typedef void (*usartRecvHandler_t)(unsigned char);
static usartRecvHandler_t handler;

extern void setRecvHandler(usartRecvHandler_t h);
extern void uart_init(void);
extern void uart_send(unsigned char c);
extern unsigned char uart_recv(void);

#endif


