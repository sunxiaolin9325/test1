/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ds18b20.h"
#include "ds_gpio.h"
#include "delay.h"

/// Sends one bit to bus
void ds18b20_send(char bit) {
  ds_gpio_set_value(0);
  delay_us(5);
  if (bit == 1)
	  ds_gpio_set_value(1);
  delay_us(80);
  ds_gpio_set_value(1);
}

// Reads one bit from bus
unsigned char ds18b20_read(void)
{
  unsigned char PRESENCE=0;
	
  ds_gpio_set_value(0);
  delay_us(2);
  ds_gpio_set_value(1);
  delay_us(15);
  if (ds_gpio_get_value() == 1)
	  PRESENCE=1;
  else 
	  PRESENCE=0;
  
  return(PRESENCE);
}

// Sends one byte to bus
void ds18b20_send_byte(char data){
  unsigned char i;
  unsigned char x;
	
  for (i=0;i<8;i++) {
    x = data>>i;
    x &= 0x01;
    ds18b20_send(x);
  }
  
  // attention
  delay_us(100);
}

// Reads one byte from bus
unsigned char ds18b20_read_byte(void)
{
  unsigned char i;
  unsigned char data = 0;
	
  for (i=0;i<8;i++) {
    if (ds18b20_read())
		data|=0x01<<i;
    delay_us(15);
  }
  return(data);
}
// Sends reset pulse
unsigned char ds18b20_RST_PULSE(void){
  unsigned char PRESENCE;
  ds_gpio_set_value(0);
  delay_us(500);
  ds_gpio_set_value(1);
  delay_us(30);
  if(ds_gpio_get_value()==0) PRESENCE=1; else PRESENCE=0;
  delay_us(470);
  if(ds_gpio_get_value()==1) PRESENCE=1; else PRESENCE=0;
  return PRESENCE;
}

// Returns temperature from sensor
float ds18b20_get_temp(void)
{
	unsigned char check;
	char temp1=0, temp2=0;
	float temp;
	
	check=ds18b20_RST_PULSE();
	if (check==1) {
		ds18b20_send_byte(0xCC);
		ds18b20_send_byte(0x44);
		delay_ms(750);
		check=ds18b20_RST_PULSE();
		ds18b20_send_byte(0xCC);
		ds18b20_send_byte(0xBE);
		temp1=ds18b20_read_byte();
		temp2=ds18b20_read_byte();
		check=ds18b20_RST_PULSE();
		temp=(float)(temp1+(temp2*256))/16;
		return temp;
	} else {
		return 0;
	}
}

void ds18b20_init()
{
	ds18b20_RST_PULSE();
	ds18b20_send_byte(0xCC);
	ds18b20_send_byte(0x4E);
	ds18b20_send_byte(0x20);
	ds18b20_send_byte(0x00);
	ds18b20_send_byte(0x7F);
	ds18b20_RST_PULSE();
}



