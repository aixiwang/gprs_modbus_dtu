
#include "USART.h"
#include "stdio.h"
#include "delay.h"
#include "gprs_lib.h"

COMx_Define	COM1,COM2;

u8	xdata TX1_buffer[COM1_TX_LENGTH];	//发送缓冲
u8 	xdata RX1_buffer[COM1_RX_LENGTH];	//接收缓冲

u8	xdata TX2_buffer[COM2_TX_LENGTH];	//发送缓冲
u8 	xdata RX2_buffer[COM2_RX_LENGTH];	//接收缓冲


void Uart1Init_115200(void)		//115200bps@22.1184MHz
{

    S1_USE_P30P31();

	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xD0;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1 
	

	REN = 1;	//允许接收
	ES  = 1;	//允许中断
	EA = 1;		//允许全局中断
}

void Uart1Init_9600(void)		//9600bps@22.1184MHz
{

    S1_USE_P30P31();

	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xC0;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

	REN = 1;	//允许接收
	ES  = 1;	//允许中断
	EA = 1;		//允许全局中断


}

void Uart1Init(void)		//9600bps@22.1184MHz
{
	Uart1Init_9600();
}

void Uart2Init(void)		//115200bps@22.1184MHz
{ 
	S2_USE_P10P11();

	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	
	UART2_INT_ENABLE();
}

u8 USART_Configuration(u8 UARTx)
{
	u8	i;	
	
	if(UARTx == USART1)
	{
		COM1.id = 1;
		COM1.TX_read    = 0;
		COM1.TX_write   = 0;
		COM1.B_TX_busy  = 0;
		COM1.RX_Cnt     = 0;
		COM1.RX_TimeOut = 0;
		COM1.B_RX_OK    = 0;

		COM1.TX_buffer=TX1_buffer;
		COM1.RX_buffer=RX1_buffer;

		for(i=0; i<COM1_TX_LENGTH; i++)
			COM1.TX_buffer[i] = 0;
		for(i=0; i<COM1_RX_LENGTH; i++)
			COM1.RX_buffer[i] = 0;

		Uart1Init();
	
		return	0;
	}
	else if(UARTx == USART2)
	{
		COM2.id = 2;
		COM2.TX_read    = 0;
		COM2.TX_write   = 0;
		COM2.B_TX_busy  = 0;

		COM2.RX_Cnt     = 0;
		COM2.RX_TimeOut = 0;
		COM2.B_RX_OK    = 0;

		COM2.TX_buffer=TX2_buffer;
		COM2.RX_buffer=RX2_buffer;

		for(i=0; i<COM2_TX_LENGTH; i++)
			COM2.TX_buffer[i] = 0;
		for(i=0; i<COM2_RX_LENGTH; i++)
			COM2.RX_buffer[i] = 0;
			
		Uart2Init();	
	}

	return 0;
}


/*************** 装载串口发送缓冲 *******************************/
 void ResetTxBuffer(u8 UARTx)
 {
    u8 i;

   if(UARTx==USART1)
   {  
     	COM1.TX_read    = 0;
		COM1.TX_write   = 0;
		COM1.B_TX_busy  = 0;	
		
		for(i=0; i<COM1_TX_LENGTH; i++)
			COM1.TX_buffer[i] = 0;	
     
   }
   else if(UARTx==USART2)
   {
   		COM2.TX_read    = 0;
		COM2.TX_write   = 0;
		COM2.B_TX_busy  = 0;	
		
		for(i=0; i<COM2_TX_LENGTH; i++)
			COM2.TX_buffer[i] = 0;	
   }
 }

void TX1_write2buff(u8 dat)	//写入发送缓冲，指针+1
{
	COM1.TX_buffer[COM1.TX_write] = dat;	//装发送缓冲

	COM1.TX_write+=1;

	if(COM1.TX_write >= COM1_TX_LENGTH)	
		COM1.TX_write = 0;

	if(COM1.B_TX_busy == 0)		//空闲
	{  
		COM1.B_TX_busy = 1;		//标志忙
		TI = 1;					//触发发送中断	
	}
}

void TX2_write2buff(u8 dat)	//写入发送缓冲，指针+1
{
	COM2.TX_buffer[COM2.TX_write] = dat;	//装发送缓冲

	COM2.TX_write+=1;

	if(COM2.TX_write >= COM2_TX_LENGTH)	
		COM2.TX_write = 0;

	if(COM2.B_TX_busy == 0)		//空闲
	{  
		COM2.B_TX_busy = 1;		//标志忙
		SET_TI2();				//触发发送中断
	}
}


void PrintString1(unsigned char *str)//发送数据缓冲区的非字符串信息，数据流信息到串口
{
  unsigned char *st_p=str;
  unsigned int i=0;
   
  while(str[i]!='\0')
  {
    //SBUF=str[i];
	 TX1_write2buff(str[i]);
	 
	 i++;
	 delay_ms(1u);
  }	  
 }

void PrintString2(unsigned char *str)//发送数据缓冲区的非字符串信息，数据流信息到串口
{
  unsigned char *st_p=str;
  unsigned int i=0;

  while(str[i]!='\0')
  {
     //S2BUF=str[i];
	 TX2_write2buff(str[i]);

	 i++;
	 delay_ms(1u);
  }  
}


void COMx_write2buff(COMx_Define *COMx, u8 dat)	//写入发送缓冲，指针+1
{
	if(COMx->id == 1)	TX1_write2buff(dat);
	if(COMx->id == 2)	TX2_write2buff(dat);
}


void SendData(u8 UARTx,u8 dat)
{
   if(UARTx == USART1)
     	TX1_write2buff(dat);
	else if(UARTx == USART2)
		TX2_write2buff(dat);
}

void SendString(u8 UARTx,u8 *puts)
{
   if(UARTx == USART1)
     	PrintString1(puts);
	else if(UARTx == USART2)
		PrintString2(puts);
}



//void SendDebug(u8* format,...)
//{       
//     va_list args;
//	 u8  buffer[100]="";
//     
//     va_start(args, format);
//     vsprintf(buffer,format,args);
//     va_end(args);
//     
//     SendString(USART1,buffer);
//}
//

void reset_uart1_rx_buff(void)
{
	u8 i;

    EA = 0;
	COM1.RX_Cnt = 0;

	for(i=0; i<COM1_RX_LENGTH; i++)
		COM2.RX_buffer[i] = 0;
	EA = 1;
}


u8 get_uart1_rx_buff_len(void)
{
	u8 i;
	EA = 0;
	i = COM1.RX_Cnt;
	EA = 1;
	return i;
}

u8 get_uart1_buff(u8 i)
{
	return COM1.RX_buffer[i];
}

/********************* UART1中断函数************************/

void UART1_int (void) interrupt UART1_VECTOR
{ 
	if(RI)
	{ 		
		RI = 0;
		if(COM1.B_RX_OK == 0)
		{
			if(COM1.RX_Cnt >= COM1_RX_LENGTH)
				COM1.RX_Cnt = 0;

			COM1.RX_buffer[COM1.RX_Cnt++] = SBUF;
			// COM1.RX_TimeOut = TimeOutSet1;
			// SendData(USART2,COM1.RX_buffer[COM1.RX_Cnt-1]);
		}
	}

	if(TI)
	{ 	   
		TI = 0;
		if(COM1.TX_read != COM1.TX_write)
		{
		 	SBUF = COM1.TX_buffer[COM1.TX_read];

			COM1.TX_read++;
			
			if(COM1.TX_read >= COM1_TX_LENGTH)	
			{
				COM1.TX_read = 0;
			}
		}
		else	
		{
			COM1.B_TX_busy = 0;
		}	
	}
	
}

/********************* UART2中断函数************************/

void UART2_int (void) interrupt UART2_VECTOR
{    

	u8 c;

	if(RI2)
	{
		CLR_RI2();
		c = S2BUF;				
		gprs_rx_int(c);
	}

	if(TI2)
	{
		CLR_TI2();		
		
		if(COM2.TX_read != COM2.TX_write)
		{
		 	S2BUF = COM2.TX_buffer[COM2.TX_read];

			COM2.TX_read++;

			if(COM2.TX_read >= COM2_TX_LENGTH)
			{
					COM2.TX_read = 0;
			}
		}
		else
		{
			COM2.B_TX_busy = 0;
		}		
	}

}



