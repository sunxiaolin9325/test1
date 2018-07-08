#include "uart.h"
#include "stm32f4xx_conf.h"

void uart_init()
{
	GPIO_InitTypeDef gpioInitValue = {0};
	USART_InitTypeDef usartInitValue = {0};
	NVIC_InitTypeDef nvicInitValue = {0};
	
	// GPIOA9 10 
	// GPIOB10 11
	// 打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// 配置GIPOx功能
	gpioInitValue.GPIO_Mode = GPIO_Mode_AF;
	gpioInitValue.GPIO_OType = GPIO_OType_PP;
	gpioInitValue.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpioInitValue.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitValue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitValue);
	
	// 选择复用引脚
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// 在stm32f4xx.h line 123 define 8000000HZ
	// 配置串口控制器
	usartInitValue.USART_BaudRate = 115200;
	usartInitValue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartInitValue.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartInitValue.USART_Parity = USART_Parity_No;
	usartInitValue.USART_StopBits = USART_StopBits_1;
	usartInitValue.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &usartInitValue);
	
	// 配置中断控制器NVIC
	nvicInitValue.NVIC_IRQChannel = USART3_IRQn;
	nvicInitValue.NVIC_IRQChannelCmd = ENABLE;
	nvicInitValue.NVIC_IRQChannelPreemptionPriority = 1;
	nvicInitValue.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvicInitValue);
	
	// 打开usart1的接收中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	// 使能USART1
	USART_Cmd(USART3, ENABLE);
}

// 不会调用
unsigned char uart_recv(void)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);
	return USART_ReceiveData(USART3);
}

void uart_send(unsigned char c)
{
	USART_SendData(USART3, (uint16_t)c);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_ClearFlag(USART3, USART_FLAG_TXE);
}

extern void buzzer_on(void);
extern void buzzer_off(void);
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
		handler((unsigned char)USART_ReceiveData(USART3));
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

void setRecvHandler(usartRecvHandler_t h)
{
	handler = h;
}




