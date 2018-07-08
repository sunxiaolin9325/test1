#include "usart4.h"
#include "stm32f4xx.h"
#include "stdio.h"
#include "stm32f4xx_conf.h"

void usart4_init(void)
{
	GPIO_InitTypeDef GPIOA_Config_Value;
	USART_InitTypeDef USART4_Config_Value;
	NVIC_InitTypeDef NVIC_Config_Value;
	
	//1.打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	//2.配置GPIO为复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	GPIOA_Config_Value.GPIO_Mode = GPIO_Mode_AF;
	GPIOA_Config_Value.GPIO_OType = GPIO_OType_PP;
	GPIOA_Config_Value.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIOA_Config_Value.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOA_Config_Value.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIOA_Config_Value);

	//3.配置串口
	USART4_Config_Value.USART_BaudRate = 115200;
	USART4_Config_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART4_Config_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART4_Config_Value.USART_Parity = USART_Parity_No;
	USART4_Config_Value.USART_StopBits = USART_StopBits_1;
	USART4_Config_Value.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4, &USART4_Config_Value);
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	//4.配置中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_Config_Value.NVIC_IRQChannel = UART4_IRQn;
	NVIC_Config_Value.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Config_Value.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Config_Value.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_Config_Value);	
	
	USART_Cmd(UART4, ENABLE);
}

void usart4_send(uint8_t c)
{	
	USART_SendData(UART4, c);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) != SET);
	USART_ClearFlag(UART4, USART_FLAG_TC);
}

void usart4_send_data(char *buf)
{
	while (*buf != '\0') {
		usart4_send(*buf);
		buf++;
	}
}

uint8_t usart4_recv(void)
{
	uint8_t c;
	
	while (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) != SET);
	c = USART_ReceiveData(UART4);
	USART_ClearFlag(UART4, USART_FLAG_RXNE);
	
	return c;
}

void clear_recv_flag(void)
{
	//清除标志位，实测发现必须先读一下标志位才可以
	USART_GetFlagStatus(UART4, USART_FLAG_RXNE);
	USART_ReceiveData(UART4);
	USART_ClearFlag(UART4, USART_FLAG_RXNE);
}

//接收输入
//专门用于接收esp8266的响应数据
//esp8266响应数据格式\r\nxxxx\r\n
//这个函数最后返回的数据是xxxx
void usart4_recv_ok(char *buf)
{
	int count = 0;
	uint8_t c;
	
	while (1) {
		c = usart4_recv();
		count++;
		//不接受前两个字符\r\n
		if ((count == 1) || (count == 2)) {
			continue;
		}
		if (c == '\n') {
			buf--;
			*buf = '\0';
			break;
		}
		*buf = c;
		buf++;
	}
}

//用户接收esp8266的连接wifi信息
//数据格式是xxxxxx\r\n
void usart4_recv_link_info(char *buf)
{
	uint8_t c;
	
	while (1) {
		c = usart4_recv();
		if (c == '\n') {
			buf--;
			*buf = '\0';
			break;
		}
		*buf = c;
		buf++;
	}
}






