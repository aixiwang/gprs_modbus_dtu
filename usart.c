
#include "USART.h"
#include "stdio.h"
#include "delay.h"
#include "gprs_lib.h"

COMx_Define	COM1,COM2;

u8	xdata TX1_buffer[COM1_TX_LENGTH];	//���ͻ���
u8 	xdata RX1_buffer[COM1_RX_LENGTH];	//���ջ���

u8	xdata TX2_buffer[COM2_TX_LENGTH];	//���ͻ���
u8 	xdata RX2_buffer[COM2_RX_LENGTH];	//���ջ���


void Uart1Init_115200(void)		//115200bps@22.1184MHz
{

    S1_USE_P30P31();

	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xD0;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1 
	

	REN = 1;	//�������
	ES  = 1;	//�����ж�
	EA = 1;		//����ȫ���ж�
}

void Uart1Init_9600(void)		//9600bps@22.1184MHz
{

    S1_USE_P30P31();

	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xC0;		//�趨��ʱ��ֵ
	TH1 = 0xFD;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

	REN = 1;	//�������
	ES  = 1;	//�����ж�
	EA = 1;		//����ȫ���ж�


}

void Uart1Init(void)		//9600bps@22.1184MHz
{
	Uart1Init_9600();
}

void Uart2Init(void)		//115200bps@22.1184MHz
{ 
	S2_USE_P10P11();

	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	
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


/*************** װ�ش��ڷ��ͻ��� *******************************/
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

void TX1_write2buff(u8 dat)	//д�뷢�ͻ��壬ָ��+1
{
	COM1.TX_buffer[COM1.TX_write] = dat;	//װ���ͻ���

	COM1.TX_write+=1;

	if(COM1.TX_write >= COM1_TX_LENGTH)	
		COM1.TX_write = 0;

	if(COM1.B_TX_busy == 0)		//����
	{  
		COM1.B_TX_busy = 1;		//��־æ
		TI = 1;					//���������ж�	
	}
}

void TX2_write2buff(u8 dat)	//д�뷢�ͻ��壬ָ��+1
{
	COM2.TX_buffer[COM2.TX_write] = dat;	//װ���ͻ���

	COM2.TX_write+=1;

	if(COM2.TX_write >= COM2_TX_LENGTH)	
		COM2.TX_write = 0;

	if(COM2.B_TX_busy == 0)		//����
	{  
		COM2.B_TX_busy = 1;		//��־æ
		SET_TI2();				//���������ж�
	}
}


void PrintString1(unsigned char *str)//�������ݻ������ķ��ַ�����Ϣ����������Ϣ������
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

void PrintString2(unsigned char *str)//�������ݻ������ķ��ַ�����Ϣ����������Ϣ������
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


void COMx_write2buff(COMx_Define *COMx, u8 dat)	//д�뷢�ͻ��壬ָ��+1
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

/********************* UART1�жϺ���************************/

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

/********************* UART2�жϺ���************************/

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



