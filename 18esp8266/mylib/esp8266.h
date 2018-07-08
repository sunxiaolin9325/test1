#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f4xx.h"

extern void esp8266_init(void);
extern int esp8266_link_wifi(char *ssid, char *passwd);
extern uint32_t esp8266_connect(char *ip, char *port);
extern uint32_t esp8266_disconnect(void);
extern uint32_t esp8266_send(char *msg, char *len);
extern uint32_t esp8266_recv(char *buf);

#endif

