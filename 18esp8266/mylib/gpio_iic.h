#ifndef __GPIO_IIC_H__
#define __GPIO_IIC_H__	

#include <stdint.h>
   	
void IIC_Init(void);                			//初始化IIC的IO口				 
void IIC_Start(void);							//发送IIC开始信号
void IIC_Stop(void);	  						//发送IIC停止信号
void IIC_Send_Byte(uint8_t data);				//IIC发送一个字节
uint8_t IIC_Recv_Byte(uint8_t ack);				//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 					//IIC等待ACK信号
void IIC_Ack(void);								//IIC发送ACK信号
void IIC_NAck(void);							//IIC不发送ACK信号

#endif


