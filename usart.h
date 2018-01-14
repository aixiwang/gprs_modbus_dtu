
#ifndef __USART_H
#define __USART_H	 

#include	"config.h"

#define	COM1_TX_LENGTH	50
#define	COM1_RX_LENGTH	50
#define	COM2_TX_LENGTH	100
#define	COM2_RX_LENGTH	100

#define	USART1	1
#define	USART2	2

#define	UART_ShiftRight	0		//同步移位输出
#define	UART_8bit_BRTx	(1<<6)	//8位数据,可变波特率
#define	UART_9bit		(2<<6)	//9位数据,固定波特率
#define	UART_9bit_BRTx	(3<<6)	//9位数据,可变波特率

#define	UART1_SW_P30_P31	0
#define	UART1_SW_P36_P37	(1<<6)
#define	UART1_SW_P16_P17	(2<<6)	//必须使用内部时钟
#define	UART2_SW_P10_P11	0
#define	UART2_SW_P46_P47	1


#define	TimeOutSet1		5
#define	TimeOutSet2		5

#define	BRT_Timer1	1
#define	BRT_Timer2	2

typedef struct
{ 
	u8	id;				//串口号

	u8	TX_read;		//发送读指针
	u8	TX_write;		//发送写指针
	u8	B_TX_busy;		//忙标志
	u8  *TX_buffer;     //发送buffer

	u8 	RX_Cnt;			//接收字节计数
	u8	RX_TimeOut;		//接收超时
	u8	B_RX_OK;		//接收块完成
	u8* RX_buffer;      //接收buffer
	 
} COMx_Define; 

typedef struct
{ 
	u8	UART_Mode;			//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//使用波特率,   BRT_Timer1,BRT_Timer2
	u32	UART_BaudRate;		//波特率,       ENABLE,DISABLE
	u8	Morecommunicate;	//多机通讯允许, ENABLE,DISABLE
	u8	UART_RxEnable;		//允许接收,   ENABLE,DISABLE
	u8	BaudRateDouble;		//波特率加倍, ENABLE,DISABLE
	u8	UART_Interrupt;		//中断控制,   ENABLE,DISABLE
	u8	UART_Polity;		//优先级,     PolityLow,PolityHigh
	u8	UART_P_SW;			//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	u8	UART_RXD_TXD_Short;	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE

} COMx_InitDefine; 

extern	COMx_Define	COM1,COM2;

u8	USART_Configuration(u8 UARTx);
void SendData(u8 UARTx,u8 dat);	//写入发送缓冲，指针+1
void SendString(u8 UARTx,u8 *puts);
void ResetTxBuffer(u8 UARTx);

void PrintString2(unsigned char *str);
void PrintString1(unsigned char *str);

//void SendDebug(u8* format,...);//打印串口调试信息
void reset_uart1_rx_buff(void);
u8 get_uart1_rx_buff_len(void);
u8 get_uart1_buff(u8 i);


#endif

