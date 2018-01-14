





#ifndef _LED_RGB_H_
#define   _LED_RGB_H_



void  led_rgb_init();

void  led_rgb_on();

void  led_rgb_off();

void  led_rgb_blink(unsigned int blinkcount,unsigned int ontime);

void  led_rgb_set(unsigned char color_red,unsigned char color_green,unsigned char color_blue);





#endif