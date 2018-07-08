#include "esp8266.h"
#include "usart4.h"
#include "string.h"
#include "uart.h"
#include "delay.h"

enum STATE_ENUM {
	STATE_O,
	STATE_K,
} state = STATE_O;

static int ok_flag = 0;

void UART4_IRQHandler(void)
{
	uint8_t c;
	
	if (USART_GetITStatus(UART4, USART_IT_RXNE) == SET) {
		c = USART_ReceiveData(UART4);
		uart_send(c);
		if (c == '>') {
			ok_flag = 1;
		}
		
		if (ok_flag == 0) {
			switch (state) {
				case STATE_O:
					if (c == 'O') {
						state = STATE_K;
					} else {
						goto end;
					}
					break;
				case STATE_K:
					if (c == 'K') {
						ok_flag = 1;
						state = STATE_O;
					} else {
						goto end;
					}
					break;
			}
		}
	}
end:	
	USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}

void esp8266_init(void)
{
	usart4_init();
}

int esp8266_link_wifi(char *ssid, char *passwd)
{
	// 重启wifi
	usart4_send_data("AT+RST\r\n");
	delay_ms(2000);
	
	//关闭回显
	usart4_send_data("ATE0\r\n");
	while (ok_flag == 0);
	
	//选择模式
	usart4_send_data("AT+CWMODE_CUR=1\r\n");
	while (ok_flag == 0);
				
	//链接wifi
	usart4_send_data("AT+CWJAP=");
	usart4_send(34); // "
	usart4_send_data(ssid);
	usart4_send(34); // "
	usart4_send(44); //,
	usart4_send(34); // "
	usart4_send_data(passwd);
	usart4_send(34); // "
	usart4_send_data("\r\n");
	while (ok_flag == 0);
	
	//必须读IP，不读的话无法建立连接
	usart4_send_data("AT+CIFSR\r\n");
	
	return 0;
}

uint32_t esp8266_connect(char *ip, char *port)
{
	ok_flag = 0;
	
	usart4_send_data("AT+CIPSTART=");
	usart4_send(34); // "
	usart4_send_data("TCP");
	usart4_send(34); // "
	usart4_send(44); //,
	usart4_send(34); // "
	usart4_send_data(ip);
	usart4_send(34); // "
	usart4_send(44); //,
	usart4_send_data(port);
	usart4_send_data("\r\n");
	while (ok_flag == 0);
	
	return 0;
}

uint32_t esp8266_disconnect(void)
{
	usart4_send_data("AT+CIPCLOSE\r\n");
	return 0;
}

uint32_t esp8266_send(char *msg, char *len)
{
	ok_flag = 0;
	
	usart4_send_data("AT+CIPSEND=");
	usart4_send_data(len);
	usart4_send_data("\r\n");	
	while (ok_flag == 0);	// OK
	
	ok_flag = 0;
	while (ok_flag == 0);	// >

	ok_flag = 0;
	usart4_send_data(msg);
	while (ok_flag == 0);
	
	return 0;
}

uint32_t esp8266_recv(char *buf)
{
	return 0;
}

