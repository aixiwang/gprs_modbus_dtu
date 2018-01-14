#ifndef _DEVICE_CTRL_H_
#define 	 _DEVICE_CTRL_H_

 #include "config.h"

void beep_init();
void beep_on();
void beep_off();
void beep_on_ms(u8 time);
void beep_on_second(u8 second);

void relay_init();
void relay_on();
void relay_off();
void relay_on_ms(u8 time);
void relay_on_second(u8 second);


#endif