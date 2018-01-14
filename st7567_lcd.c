   

//#include "reg51.h"
//#include "absacc.h"
#include "delay.h"
//#include "spi.h"
#include "st7567_lcd.h"

																		  
//sbit    RSTB    =P2^0;	  
//sbit    A0      =P1^7;
//sbit    CS0     =P1^2;
//
//sbit    SDI     =P1^3;
//sbit	SCL		=P1^5;

#define     RSTB    P20
#define     A0      P17
#define     CS0     P12
#define     SDI     P13
#define 	SCL		P15


//void Write_ST7567_DISPLAY_DATA_serial_4SPI(unsigned char DADA_4SPI);
void Write_ST7567_COM(unsigned char COMDADA_4SPI);


   //--  宽度x高度=128x64  --
  //打电话图片16*16 ,开始打电话...
  /*
const u8 dial_pic[]={
0x00,0x00,0x00,0x00,0x07,0x00,0xC0,0x3F,0x00,0xE0,0x7F,0x00,0xE0,0xFF,0x00,0xC0,
0xFF,0x01,0x80,0xE7,0x03,0x00,0xC3,0x07,0x00,0x80,0x0F,0x16,0x00,0x1F,0xB6,0x0D,
0x1E,0x24,0x09,0x3C,0x24,0x19,0x3C,0x64,0x22,0x3E,0x4C,0x06,0x7F,0xC8,0x38,0x7F,
0x90,0x61,0x3E,0x30,0x07,0x3C,0x60,0x3C,0x18,0xC0,0x20,0x00,0x80,0x03,0x00,0x00,
0x3E,0x00,0x00,0x20,0x00,0x00,0x00,0x00};
*/
const u8 idata support_font[]="0123456789*# ." ;

#define   MAIN_FONT_WIDTH    8
#define   MAIN_FONT_HEIGHT   16

 //宽度为8，高度为16 ,每行最多显示16个字符，最多8行
const u8 xdata font_8x16_lib[]={
0x00,0x00,0xE0,0x0F,0x10,0x10,0x08,0x20,0x08,0x20,0x10,0x10,0xE0,0x0F,0x00,0x00,/*"0",0*/
0x00,0x00,0x10,0x20,0x10,0x20,0xF8,0x3F,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,/*"1",1*/
0x00,0x00,0x70,0x30,0x08,0x28,0x08,0x24,0x08,0x22,0x88,0x21,0x70,0x30,0x00,0x00,/*"2",2*/
0x00,0x00,0x30,0x18,0x08,0x20,0x88,0x20,0x88,0x20,0x48,0x11,0x30,0x0E,0x00,0x00,/*"3",3*/
0x00,0x00,0x00,0x07,0xC0,0x04,0x20,0x24,0x10,0x24,0xF8,0x3F,0x00,0x24,0x00,0x00,/*"4",4*/
0x00,0x00,0xF8,0x19,0x08,0x21,0x88,0x20,0x88,0x20,0x08,0x11,0x08,0x0E,0x00,0x00,/*"5",5*/
0x00,0x00,0xE0,0x0F,0x10,0x11,0x88,0x20,0x88,0x20,0x18,0x11,0x00,0x0E,0x00,0x00,/*"6",6*/
0x00,0x00,0x38,0x00,0x08,0x00,0x08,0x3F,0xC8,0x00,0x38,0x00,0x08,0x00,0x00,0x00,/*"7",7*/
0x00,0x00,0x70,0x1C,0x88,0x22,0x08,0x21,0x08,0x21,0x88,0x22,0x70,0x1C,0x00,0x00,/*"8",8*/
0x00,0x00,0xE0,0x00,0x10,0x31,0x08,0x22,0x08,0x22,0x10,0x11,0xE0,0x0F,0x00,0x00,/*"9",9*/
0x40,0x02,0x40,0x02,0x80,0x01,0xF0,0x0F,0x80,0x01,0x40,0x02,0x40,0x02,0x00,0x00,/*"*",10*/
0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0xC0,0x3F,0x78,0x04,0x40,0x04,0x00,0x00,/*"#",11*/	
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",SPACE*/
0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*".",7*/
}; 

//a-z,A-Z保存在EEPROM里,保存地址如下0-0x19F是a-z,0x1A0-0x33F是A-Z

u8 idata common_buffer[16];

//scrreen 8 page,从上到下分为8个page行，每个page里128列；

CURSOR_POSITION idata g_cursorPos={0,0};

SCREEN_POSITION cursor2Screen(CURSOR_POSITION cursor)
{
  	 SCREEN_POSITION 	screen;
   
	  screen.column=cursor.x*MAIN_FONT_WIDTH;
	  screen.page=cursor.y*2;

	  return   screen;
}
  

void cursorNext()
{
   	g_cursorPos.x++;

	g_cursorPos.y+=g_cursorPos.x/16;
	g_cursorPos.x=g_cursorPos.x%16;
	
	g_cursorPos.y=g_cursorPos.y%4;	
}

void cursorPre()
{
    if(g_cursorPos.x==0 && g_cursorPos.y==0)
		return;
	
   	g_cursorPos.x--;
	if(g_cursorPos.x<0)
	{ 	  
	   	g_cursorPos.y--;

		g_cursorPos.x+=16;
	}	
}

CURSOR_POSITION getCursorPos()
{
   return 	g_cursorPos;
}

 void setCursorPos(CURSOR_POSITION cursor)
 {
    g_cursorPos.x= cursor.x;
	g_cursorPos.y= cursor.y;
   
 }

 void reset_cursor()
 {
   g_cursorPos.x=0;
   g_cursorPos.y=0;
 }

//--------------------------------------------------------------------------
/*
void	SPI_config(void)
{
	SPI_InitTypeDef		SPI_InitStructure;
	SPI_InitStructure.SPI_Module    = ENABLE;              //SPI启动    ENABLE, DISABLE
	SPI_InitStructure.SPI_SSIG      = ENABLE;			  //片选位     ENABLE, DISABLE
	SPI_InitStructure.SPI_FirstBit  = SPI_MSB;			  //移位方向   SPI_MSB, SPI_LSB
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;	  //主从选择   SPI_Mode_Master, SPI_Mode_Slave
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;      //时钟相位   SPI_CPOL_High,   SPI_CPOL_Low
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;	  //数据边沿   SPI_CPHA_1Edge,  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_Interrupt = ENABLE;			  //中断允许   ENABLE,DISABLE
	SPI_InitStructure.SPI_Speed     = SPI_Speed_16;		  //SPI速度    SPI_Speed_4, SPI_Speed_16, SPI_Speed_64, SPI_Speed_128
	SPI_InitStructure.SPI_IoUse     = SPI_P12_P13_P14_P15; //IO口切换   SPI_P12_P13_P14_P15, SPI_P24_P23_P22_P21, SPI_P54_P40_P41_P43
	SPI_Init(&SPI_InitStructure);
	
	SPI_TxRxMode = SPI_Mode_Slave;

	SPI_Init(&SPI_InitStructure);

	EA=1;	
} */



void Init_ST7567_LCD()
{      
       u8 j;
	//   SPI_config(); 	   

	   HDReset();

	   delay_ms(100);

       Write_ST7567_COM(0xe2);	  //SOFT RESET//寄存器复位

       Write_ST7567_COM(0xaf);	  //0xaf显示器开
       Write_ST7567_COM(0x2f);	  //0x2f升压电路,电压管理电路,
	   	   
       Write_ST7567_COM(0x25);	   //0x20-0x27为V5电压内部电阻调整设置 

       Write_ST7567_COM(0x81);	   // SET EV 调对比度
       Write_ST7567_COM(0x1F);	   //0x01-0x3f电量寄存器设置模式

       Write_ST7567_COM(0xa0);	   //0xa0为Segment正向,0xa1 为Segment反向
	   	   
       Write_ST7567_COM(0xc8);	   //0xc0正向扫描,0xc8反射扫描

       Write_ST7567_COM(0xa6);	   // //0xa6正向显示,0xa7反向显示
       Write_ST7567_COM(0xa4);	   // //0xa4正常显示,0xa5全屏点亮 ,0xae全屏关闭

 //      Write_ST7567_COM(0xac);	   //
 //      Write_ST7567_COM(0x00);	

       Write_ST7567_COM(0xf8);	   // //背压比设置
       Write_ST7567_COM(0x00);		 //00--10

	   Write_ST7567_COM(0xa6);	   // //0xa6正向显示,0xa7反向显示

	   Write_ST7567_COM(0xb0+8);	 //SET PAGE 0---8
	   Write_ST7567_COM(0x10+0);	 //SET COLUMN MSB	   
	   Write_ST7567_COM(0x00+0);	 //SET CLUMN LSB
	
	   for(j=0;j<128;j++)
	   {
	 	 Write_ST7567_DISPLAY_DATA_serial_4SPI(0xff);	
	   } 
}

//void display_dial_pic(u8 bOn)
//{
//	u8 j,page,msb,lsb,column;
//	static u8 dotnum=0;
//
//	page=3;
//	column=30;
//
//	msb=column>>4;
//	lsb=column&0x0f;
//		
//	Write_ST7567_COM(0xb0+page);
//    Write_ST7567_COM(0x10+msb);
//	Write_ST7567_COM(0x00+lsb);	 
//	
//	for(j=0;j<72;j+=3)
//	{
//		if(bOn==1)
//			Write_ST7567_DISPLAY_DATA_serial_4SPI(dial_pic[j]); 			
//		else
//			Write_ST7567_DISPLAY_DATA_serial_4SPI(0x00); 			
//	}
//
//	delay_ms(1);
//	Write_ST7567_COM(0xb0+page+1);
//    Write_ST7567_COM(0x10+msb);
//	Write_ST7567_COM(0x00+lsb); 	
//	for(j=1;j<72;j+=3)
//	{
//	    if(bOn==1)
//			Write_ST7567_DISPLAY_DATA_serial_4SPI(dial_pic[j]);			
//		else
//		    Write_ST7567_DISPLAY_DATA_serial_4SPI(0x00); 
//	}
//
//	delay_ms(1);
//	Write_ST7567_COM(0xb0+page+2);
//    Write_ST7567_COM(0x10+msb);
//	Write_ST7567_COM(0x00+lsb);
//	for(j=2;j<72;j+=3)
//	{
//		if(bOn==1)
//			Write_ST7567_DISPLAY_DATA_serial_4SPI(dial_pic[j]);			
//		else
//		    Write_ST7567_DISPLAY_DATA_serial_4SPI(0x00); 		
//	}
//
//	if(bOn==1)	
//		dotnum++;
//	else
//	    dotnum=10;//实际上是擦除
//
//	j=0;
//	while(j<dotnum)
//	{
//		displayCharWidthScreenPos(page+1,column+30+j*8,'.');
//
//		j++;
//	}
//
//	if(	dotnum>5)
//	{
//	    j=0;
//		while(j<dotnum)
//		{
//			displayCharWidthScreenPos(page+1,column+30+j*8,' ');
//	
//			j++;
//		}
//
//		dotnum=0;
//	}
//	
//} 

void clear_screen(unsigned char xx)
{
	unsigned char i,j;

	CS0=0;

	for(i=0;i<8;i++)
	{	
		Write_ST7567_COM(0xb0+i);	 //SET PAGE 0---8
	    Write_ST7567_COM(0x10);	 	//SET COLUMN MSB	   
		Write_ST7567_COM(0x00);	  	//SET CLUMN LSB
		for(j=0;j<128;j++)
		{
			Write_ST7567_DISPLAY_DATA_serial_4SPI(xx);
		}
	 }
}

//x 0-15,y,0-3
void displayChar(u8 chr)
{
   u8 i,j,msb,lsb;
   u16 adressd=0;
   CURSOR_POSITION curPos;
   SCREEN_POSITION scrPos;
	
	i=0;
	while(support_font[i]!='\0')
	{
	  if(support_font[i]==chr)
	      break;
	  i++;
	}
   
   if(support_font[i]=='\0')
    return;

	curPos=getCursorPos();
	scrPos=cursor2Screen(curPos);

	Write_ST7567_COM(0xb0+scrPos.page);	 //SET PAGE 0---8
	
	msb=(scrPos.column&0x00ff);	
	msb>>=4;
	lsb=(scrPos.column&0x000f);

	Write_ST7567_COM(0x10+msb);	 	//SET COLUMN MSB 		   
	Write_ST7567_COM(0x00+lsb);	  	//SET CLUMN LSB

	adressd=i*16;
	for(j=0;j<16;j+=2)
	{
		Write_ST7567_DISPLAY_DATA_serial_4SPI(font_8x16_lib[adressd+j]);
	}

	Write_ST7567_COM(0xb0+scrPos.page+1);	 //SET PAGE 0---8
	Write_ST7567_COM(0x10+msb);	 //SET COLUMN MSB	   
	Write_ST7567_COM(0x00+lsb);	 //SET CLUMN LSB
	
	for(j=1;j<16;j+=2)
	{
		Write_ST7567_DISPLAY_DATA_serial_4SPI(font_8x16_lib[adressd+j]);	
	}

//	cursorNext(); 
}

void displayCharWidthScreenPos(u8 page,u8 column,u8 chr)
{
   u8 i,j,msb,lsb;
   u16 adressd=0;
   u8* font_lib;

   if((chr>='a' && chr<='z') ||
      (chr>='A' && chr<='Z'))
	 {     
	     if(chr>='a' && chr<='z')
		  {
			 EEPROM_read_n((chr-'a')*16+0x000,common_buffer,16);			 
		  }
		  else 
		  {
		     EEPROM_read_n((chr-'A')*16+0x19F,common_buffer,16);		
		  }
		 // for(i=0;i<16;i++)
		 //	 SendData(USART1,common_buffer[i]);

		  font_lib=common_buffer;
		  adressd=0;
	 }
	 else
	 { 	
		i=0;
		while(support_font[i]!='\0')
		{
		  if(support_font[i]==chr)
		      break;
		  i++;
		}
	   
	   if(support_font[i]=='\0')
	    return;	

		adressd=i*16;
		font_lib= font_8x16_lib;
	}

	Write_ST7567_COM(0xb0+page);	 //SET PAGE 0---8
	
	msb=(column&0x00ff);	
	msb>>=4;
	lsb=(column&0x000f);

	Write_ST7567_COM(0x10+msb);	 	//SET COLUMN MSB 		   
	Write_ST7567_COM(0x00+lsb);	  	//SET CLUMN LSB
	
	for(j=0;j<16;j+=2)
	{
		Write_ST7567_DISPLAY_DATA_serial_4SPI(font_lib[adressd+j]);
	}

	Write_ST7567_COM(0xb0+page+1);	 //SET PAGE 0---8
	Write_ST7567_COM(0x10+msb);	 //SET COLUMN MSB	   
	Write_ST7567_COM(0x00+lsb);	 //SET CLUMN LSB
	
	for(j=1;j<16;j+=2)
	{
		Write_ST7567_DISPLAY_DATA_serial_4SPI(font_lib[adressd+j]);	
	} 
}

void displayStringWidthScreenPos(u8 page,u8 column,u8* pString)
{
 	 int i=0;
	 u8 uPage,uCol;		

	 uPage=page;
	 uCol=column;
	 while(pString[i]!='\0')
	 {
	   displayCharWidthScreenPos(uPage,uCol,pString[i]);	   

	   uCol+=8;
	   if(uCol>=120)
	   {
	     uPage++;
		 if(uPage>=7)
		 {
		   uPage=0;
		 }
	   }

	   i++;
	 }
}

void displayString(u8* pString)
{
 	 int i=0;
	 CURSOR_POSITION curPos;

	  curPos=getCursorPos();

	 while(pString[i]!='\0')
	 {
	   displayChar(pString[i]);
	   cursorNext(); 

	   i++;
	 }

	 setCursorPos(curPos);
}

void inputChar(u8 chr)
{	
  displayChar(chr);
  cursorNext(); 
} 

void inputString(u8* pString)
{
 	 int i=0;

	 while(pString[i]!='\0')
	 {
	   displayChar(pString[i]);
	   cursorNext(); 

	   i++;
	 }
}

void inputBackspace()
{
  	cursorPre();
	displayChar(' ');
}

//***************************************************************************SPI4
/*
void Write_ST7567_COM(unsigned char COMDADA_4SPI)
{	
	A0=0;	
	SPI_WriteToTxBuf(COMDADA_4SPI);
	SPI_TrigTx();			
}

void Write_ST7567_DISPLAY_DATA_serial_4SPI(unsigned char DADA_4SPI)
{   
	A0=1;	
	SPI_WriteToTxBuf(DADA_4SPI);
	SPI_TrigTx();			
}
*/
void Write_ST7567_COM(unsigned char COMDADA_4SPI)
{
	unsigned char bit8;
	A0=0;
	CS0=0;
	for(bit8=0;bit8<8;bit8++)
	{
		SCL=0;

		if((COMDADA_4SPI&0x80)==0x80)
		SDI=1;
		else		
		SDI=0;

//		SCL=0;
		SCL=1;
		COMDADA_4SPI=(COMDADA_4SPI<<1);

	}
  //  CS1=1;  			
}

void Write_ST7567_DISPLAY_DATA_serial_4SPI(unsigned char DADA_4SPI)
{   
	unsigned char bit8;
	A0=1;
	CS0=0;				

	for(bit8=0;bit8<8;bit8++)
	{
		SCL=0;

		if((DADA_4SPI&0x80)==0x80)
		SDI=1;
		else
		SDI=0;

		SCL=1;
		DADA_4SPI=(DADA_4SPI<<1);

	}
//	CS1=1;
}

void HDReset()
{
	RSTB=0;

	delay_ms(2);

	RSTB=1;
	delay_ms(4);
}

         	