//--------------------------------------
// UTILS_LIB
// Copyright by Aixi Wang
//--------------------------------------
#include "config.h"

//------------------------
// uart1_tx
//------------------------
void uart1_tx(u8 c)
{
	SendData(USART1,c);
}

//------------------------
// uart2_tx
//------------------------
void uart2_tx(u8 c)
{
	SendData(USART1,c);
}


//------------------------
// uart1_puts
//------------------------
void uart1_puts(u8 *s)
{
	PrintString1(s);
}

//------------------------
// uart2_puts
//------------------------
void uart2_puts(u8 *s)
{
	PrintString2(s);
}



//------------------
// lcd_clr
//------------------
void lcd_clr(void)
{
    clear_screen(0x00);
}


//------------------
// lcd_putstr
//------------------
void lcd_putstr(u8* pstr)
{
	displayStringWidthScreenPos(0,0,pstr);  	
}

//------------------
// lcd_puthex
//------------------
void lcd_puthex(u8* pstr,u8 n)
{
	int i;
	u8 lcd_buff[17];

	if (n > 8)
		return;
	
	for(i=0; i<n; i++)
	{
		lcd_buff[i*2] = hexit(pstr[i] >> 4);
		lcd_buff[i*2+1] = hexit(pstr[i] & 0xf);
	}
	lcd_buff[i*2] = 0;
	displayStringWidthScreenPos(2,0,lcd_buff);  	

}

//------------------
// lcd_puthex_2
//------------------
void lcd_puthex_2(u8* pstr,u8 n)
{
	int i;
	u8 lcd_buff[17];

	if (n > 8)
		return;
	
	for(i=0; i<n; i++)
	{
		lcd_buff[2*i] = hexit(pstr[i] >> 4);
		lcd_buff[2*i + 1] = hexit(pstr[i] & 0xf);
	}
	lcd_buff[i*2] = 0;
	displayStringWidthScreenPos(4,0,lcd_buff);  	

}

//------------------
// lcd_putstr2
//------------------
void lcd_putstr2(u8 x,u8 y,u8* pstr)
{
	displayStringWidthScreenPos(x,y,pstr);  	
}



