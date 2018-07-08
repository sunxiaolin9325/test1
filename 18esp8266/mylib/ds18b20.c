#include "ds18b20.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "ds_gpio.h"

int  DS1820_Reset(void);                //DS1820 复位 
void DS1820_WriteData(char wData);    	//写数据到 DS1820 

/********************************************************** 
 *DS18B20 初始化 
 *函数名称:DS1820_WriteData() 
 *说明:本初始化程序可以不要,因为 18B20 在出厂时就被配置为 12 位精度了 
 **********************************************************/ 
void DS18B20_Init(void)     
{ 
	DS1820_Reset(); 
	DS1820_WriteData(0xCC);    //  跳过 ROM 
	DS1820_WriteData(0x4E);    //  写暂存器 
	DS1820_WriteData(0x20);    //  往暂存器的第三字节中写上限值 
	DS1820_WriteData(0x00);    //  往暂存器的第四字节中写下限值 
	DS1820_WriteData(0x7F);    //  将配置寄存器配置为 12 位精度 

	DS1820_Reset(); 
} 

/********************************************************** 
 *DS1820 复位及存在检测(通过存在脉冲可以判断 DS1820 是否损坏) 
 *函数名称:DS1820_Reset() 
 *说明:函数返回一个位标量(0 或 1)flag=0 存在,反之 flag=1 不存在 
 **********************************************************/ 
int DS1820_Reset(void) 
{ 
	int flag;

	set_ds_gpio_value(0);
	//延时 480 微秒,产生复位脉冲 
	delay_us(480);
	set_ds_gpio_value(1);
	//延时 80 微秒对总线采样 
	delay_us(80);
	flag = get_ds_gpio_value();   
	//对数据脚采样 
	delay_us(400);    //延时 400 微秒等待总线恢复 

	return flag;                //根据 flag 的值可知 DS1820 是否存在或损坏  ,可加声音告警提示 DS1820 故障 
} 

/********************************************************** 
 *写数据到 DS1820 
 *函数名称:DS1820_WriteData() 
 **********************************************************/ 
void DS1820_WriteData(char wData) 
{ 
	char i; 

	for(i = 8; i > 0; i--){
		set_ds_gpio_value(0);                  //拉低总线,产生写信号 
		delay_us(4);            		//延时 4us 
		set_ds_gpio_value(wData & 0x01); 	//发送 1 位 
		delay_us(60);          		//延时 60us,写时序至少要 60us 
		set_ds_gpio_value(1);                  //释放总线,等待总线恢复 
		wData >>= 1;                    //准备下一位数据的传送 
	} 
} 

/********************************************************** 
 *从 DS1820 中读出数据 
 *函数名称:DS1820_ReadData() 
 **********************************************************/ 
char DS1820_ReadData(void) 
{ 
	char i,TmepData = 0; 

	for(i = 8; i > 0; i--){
		TmepData >>= 1; 
		set_ds_gpio_value(0);                  //拉低总线,产生读信号 
		delay_us(4);              	//延时 4us 
		set_ds_gpio_value(1);                  //释放总线,准备读数据 
		delay_us(8);              	//延时 8 微秒读数据 
		if(get_ds_gpio_value())
			TmepData |= 0x80; 
		delay_us(60);            		//延时 60us 
		set_ds_gpio_value(1);                  //拉高总线,准备下一位数据的读取. 
	} 
	return TmepData;			//返回读到的数据 
} 

/********************************************************** 
 *转换子程序 
 **********************************************************/ 
void tem_to_string(char *buf, char temperature[]) 
{ 
	unsigned char temp_data,temp_data_2; 
	unsigned short TempDec; 		//用来存放 4 位小数 

	temp_data = temperature[1]; 
	temp_data &= 0xf0;    			//取高 4 位 

	if(temp_data == 0xf0){ 			//判断是正温度还是负温度读数
						//负温度读数求补,取反加 1,判断低 8 位是否有进位 
		if(temperature[0]==0){ 		//有进位,高 8 位取反加 1 
			temperature[0]=~temperature[0]+1; 
			temperature[1]=~temperature[1]+1; 
		}else{ 				//没进位,高 8 位不加 1 
			temperature[0]=~temperature[0]+1; 
			temperature[1]=~temperature[1]; 
		} 
	} 
	//温度格式  temperature[1]:[xxxxAAAA] AAAA 温度的高4位
	//温度格式  temperature[0]:[BBBBCCCC] BBBB 温度的低4位 CCCC小数（乘以0.0625得到的是温度）
	temp_data = temperature[1]<<4;      						//取高字节低 4 位(温度读数高 4 位),注意此时是 12 位精度 
	temp_data_2 = temperature[0]>>4; 								//取低字节高 4 位(温度读数低 4 位),注意此时是 12 位精度 
	temp_data = temp_data | temp_data_2; 						//组合成完整数据 

	buf[0] = temp_data / 100 + 0x30;      					//取百位转换为 ASCII 码 
	buf[1] = (temp_data % 100) / 10 + 0x30;    			//取十位转换为 ASCII 码 
	buf[2] = (temp_data % 100 ) % 10 + 0x30;    		//取个位转换为 ASCII 码 
	buf[3] = '.';

	temperature[0] &= 0x0f;                        	//取小数位转换为 ASCII 码 
	TempDec = temperature[0] * 625;                	//625=0.0625*10000,表示小数部分,扩大 1 万倍  ,方便显示 
	buf[4] = TempDec / 1000 + 0x30;                	//取小数十分位转换为 ASCII 码 
	buf[5] = (TempDec % 1000) / 100 + 0x30;      		//取小数百分位转换为 ASCII 码 
	buf[6] = ((TempDec % 1000) % 100) / 10 + 0x30;	//取小数千分位转换为 ASCII 码 
	buf[7] = ((TempDec % 1000) % 100) % 10 + 0x30;	//取小数万分位转换为 ASCII 码 
	buf[8] = '\0';
}

void get_temperature(char *buf) 
{ 
	int i; 
	char temperature[2]; //存放温度数据 

	DS1820_Reset();                  //复位 
	DS1820_WriteData(0xcc); //跳过 ROM 命令 
	DS1820_WriteData(0x44); //温度转换命令 
	set_ds_gpio_value(1);
	delay_ms(750);
	DS1820_Reset();                  //复位 
	DS1820_WriteData(0xcc); //跳过 ROM 命令 
	DS1820_WriteData(0xbe); //读 DS1820 温度暂存器命令 
	for (i=0;i<2;i++){ 
		temperature[i] = DS1820_ReadData();    //采集温度 
	} 
	DS1820_Reset();              //复位,结束读数据 
	tem_to_string(buf, temperature);
	delay_us(50);
} 


