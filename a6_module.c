

#include	"a6_module.h"


sbit  MODULE_RST	     =P3^4 ;
sbit  MODULE_POWER_KEY	 =P3^5 ;

void a6_module_init()
{
	MODULE_RST=0;
	MODULE_POWER_KEY=1;	 
	
	a6_module_on();
		 
}

void   a6_module_on()
{
    
	MODULE_RST=0;
	MODULE_POWER_KEY=1;		
//	delay_ms(3000); 

	//MODULE_POWER_KEY=0;
}

void   a6_module_reset()
{    
   a6_module_off();
   delay_ms(100); 
   a6_module_on();
   delay_ms(100); 


}

void   a6_module_off()
{     
   MODULE_POWER_KEY=0;
   MODULE_RST=1;
  // delay_ms(3000); 
}