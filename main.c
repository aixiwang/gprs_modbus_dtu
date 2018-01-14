//------------------------------
// GPRS_DTU_A6
//
// MCU: STC15L2K32S2
// 
// Copyright by Aixi Wang 2016 (aixi.wang@hotmail.com)
//------------------------------

#include	"config.h"
#include	"delay.h"
#include	"usart.h"
#include    "st7567_lcd.h"
// #include 	"key_board.h"
// #include    "led_rgb.h"
#include    "a6_module.h"
#include    "stdarg.h"
#include 	"gprs_lib.h"
#include 	"modbus_lib.h"
#include   "utils_lib.h"

void t_5sec_hook(void);
void t_10sec_hook(void);
void t_10min_hook(void);
void t_1min_hook(void);
void do_idle(void);
void do_sensor_task(void);

u16 count_s5 = 0;
u16 count_s10 = 0;
u16 count_10min = 0;
u16 count_1min = 0;

extern u32 xdata sensor_data[SENSOR_DATA_LEN];

u8 xdata modbus_buff[64];


//------------------
// Timer0Init
//------------------
void Timer0Init(void)		//10����@22.1184MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xB8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}


void Timer3Init(void)		//5����@22.1184MHz
{
	T4T3M &= 0xFD;		//��ʱ��ʱ��12Tģʽ
	T3L = 0x00;		//���ö�ʱ��ֵ
	T3H = 0xDC;		//���ö�ʱ��ֵ
	T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
}

//------------------
// timer3_int
//------------------
void timer3_int (void) interrupt TIMER3_VECTOR
{
	count_s5++;
	count_s10++;
	count_1min++;
	
	uart1_tx('T');
}





//------------------
// gpio_init
//------------------
void gpio_init()
{
   	//P3M1 = 0x00;
	//P3M0 = 0xff;

   	P0M1 =0x00 ;
   	P0M0 =0x00 ;

	P1M1 =0x00 ;
   	P1M0 =0xC0 ;

   	P2M1 =0x00 ;
   	P2M0 =0x00 ; 

	P5M1 = 0x00;
	P5M0 = 0xff;
}



//--------------------
// main
//--------------------
				
void main(void)
{	

	u8 i=0;	
	u8  bClearScreen=0;
	u8  bCldStart=0;


	USART_Configuration(USART1);
	USART_Configuration(USART2);

	gpio_init();
	beep_init();  
	Init_ST7567_LCD();

	lcd_clr();

	lcd_putstr("0001");  			

	// beep_on_ms(500);



	count_s5 = 0;
	count_s10 = 0;
	count_1min = 0;
	count_10min = 0;

	// start timer
	// Timer0Init();
	// Timer3Init();

	do_sensor_task();


	while(1)
	{ 		  
		   if(count_s5> 5)//5���ּ�ʱ��
		   {
		      t_5sec_hook();
      	      count_s5=0;
		   }

		   if(count_s10 > 10)//10���ּ�ʱ��
		   {	       	
		       t_10sec_hook();
			     count_s10=0;
		   } 

		   if(count_1min >60)//60���ּ�ʱ��
		   {	       	
		      t_1min_hook();
			    count_1min=0;


		   } 

		   if(count_10min >600)//600���ּ�ʱ��
		   {	       	
		      t_10min_hook();
			  count_10min=0;
		   } 


		   do_idle();
	}  
}

//--------------------
// read_ys8156b_anqi
//--------------------
int read_ys8156b_anqi(u8 addr,u32* pu32)
{
	u8 pkg_len;
	int ret;
	int i;

	reset_uart1_rx_buff();

	ret = create_03_pkg(modbus_buff,addr,0,2,&pkg_len);
	if (ret < 0)
		return ret;

	lcd_putstr("0002");
	hexdump(modbus_buff,pkg_len);  // DEBUG ONLY

	for (i=0; i<pkg_len; i++)
		uart1_tx(modbus_buff[i]);	

	ret = read_03_response_pkg(modbus_buff,addr,2);

	if (ret < 0){
		// hexdump((u8*)&ret,2);  // DEBUG ONLY
		return ret;
	}

	lcd_putstr("F003");


    *pu32 = (u32)(modbus_buff[3]*256 + modbus_buff[4]);

	return 0;

}

//--------------------
// do_sensor_task
//--------------------
void do_sensor_task(void)
{
   int retcode;
   int n;


   sensor_data[0] = 0x1000;
   sensor_data[1] = 2222;
   sensor_data[2] = 3333;
   sensor_data[3] = 4444;

   retcode = read_ys8156b_anqi(0x01,&sensor_data[0]);	

   if (retcode == 0)
   {
	  lcd_putstr("0003");
   }
   else
   {
      beep_on_ms(500);
	  delay_ms(500);
      beep_on_ms(500);
	  delay_ms(500);
      beep_on_ms(500);
	  delay_ms(500);
	  return;
   }


  a6_module_reset();
   n = 3;
   while (n-- >0)
   {
	   retcode = gprs_send("115.29.178.81",10000,"GPRS001",sensor_data,4);
	   if (retcode == 0)
	   {
		  lcd_putstr("0004"); 
 
		  beep_on_ms(100);
		  break;
	   }
	   else
		  lcd_putstr("F014");  
   }
	
   delay_ms(1000);
   a6_module_off();
	


}


//--------------------
// t_5sec_hook
//--------------------
void t_5sec_hook(void)
{
   lcd_putstr("0100");
}

//--------------------
// t_10sec_hook 
//--------------------
void t_10sec_hook(void)
{
   lcd_putstr("0200");
}

//--------------------
// t_1min_hook 
//--------------------
void t_1min_hook(void)
{
   lcd_putstr("0300");

   do_sensor_task();
}

//--------------------
// t_10min_hook 
//--------------------
void t_10min_hook(void)
{
   lcd_putstr("0400");
}



//------------------
// do_idle 
//------------------
void do_idle(void)
{
	delay_ms(1000);
	count_s5++;
	count_s10++;
	count_1min++;
	count_10min++;


}


