

#include "config.h"

sbit LED_RED=P2^3;
sbit LED_GREEN=P2^2;
sbit LED_BLUE=P2^1;



void  led_rgb_init()
{
	LED_RED=0;	
	LED_GREEN=0;
	LED_BLUE=0;	
}


void  led_rgb_on()
{
	LED_RED=1;
	LED_GREEN=1;
	LED_BLUE=1;	
	
}

void  led_rgb_off()
{
	LED_RED=0;	
	LED_GREEN=0;
	LED_BLUE=0;		
}


void  led_rgb_set(unsigned char color_red,unsigned char color_green,unsigned char color_blue)
{
	LED_RED=color_red;
	LED_GREEN=color_green;
	LED_BLUE=color_blue;			
}


void  led_rgb_blink(unsigned int blinkcount,unsigned int ontime)
{
    unsigned int  i=blinkcount;
	unsigned int pwmtime=1000;
	

	while(i>0)
	{
    	led_rgb_on();
		delay_ms(ontime*1000/100);
		led_rgb_off() ;
		delay_ms((100-ontime)*1000/100);

		i--;
	}

	 
}