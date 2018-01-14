


#include "device_ctrl.h"


sbit  BEEP          =P5^5;
sbit  RELAY_CTRL    =P1^6;

void beep_init()
{
	BEEP=0;
}

void beep_on()
{
   BEEP=1;
}


void beep_off()
{
	BEEP=0;
}

void beep_on_ms(u8 ms)
{
   BEEP=1;

   delay_ms(ms);

   BEEP=0;
} 

void beep_on_second(u8 second)
{
   BEEP=1;

   delay_ms(second*1000);

   BEEP=0;
} 

void relay_init()
{
	RELAY_CTRL=0;
}

void relay_on()
{
   RELAY_CTRL=1;
}

void relay_off()
{
   RELAY_CTRL=0;
}

void relay_on_ms(u8 time)
{
   RELAY_CTRL=1;  

   delay_ms(time);

   RELAY_CTRL=0;
}

void relay_on_second(u8 second)
{
   RELAY_CTRL=1;  

   delay_ms(second*1000);

   RELAY_CTRL=0;
}
