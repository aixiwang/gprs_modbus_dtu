
#ifndef __USART_H
#define __USART_H	 

#include	"config.h"

#define	COM1_TX_LENGTH	50
#define	COM1_RX_LENGTH	50
#define	COM2_TX_LENGTH	100
#define	COM2_RX_LENGTH	100

#define	USART1	1
#define	USART2	2

#define	UART_ShiftRight	0		//ͬ����λ���
#define	UART_8bit_BRTx	(1<<6)	//8λ����,�ɱ䲨����
#define	UART_9bit		(2<<6)	//9λ����,�̶�������
#define	UART_9bit_BRTx	(3<<6)	//9λ����,�ɱ䲨����

#define	UART1_SW_P30_P31	0
#define	UART1_SW_P36_P37	(1<<6)
#define	UART1_SW_P16_P17	(2<<6)	//����ʹ���ڲ�ʱ��
#define	UART2_SW_P10_P11	0
#define	UART2_SW_P46_P47	1


#define	TimeOutSet1		5
#define	TimeOutSet2		5

#define	BRT_Timer1	1
#define	BRT_Timer2	2

typedef struct
{ 
	u8	id;				//���ں�

	u8	TX_read;		//���Ͷ�ָ��
	u8	TX_write;		//����дָ��
	u8	B_TX_busy;		//æ��־
	u8  *TX_buffer;     //����buffer

	u8 	RX_Cnt;			//�����ֽڼ���
	u8	RX_TimeOut;		//���ճ�ʱ
	u8	B_RX_OK;		//���տ����
	u8* RX_buffer;      //����buffer
	 
} COMx_Define; 

typedef struct
{ 
	u8	UART_Mode;			//ģʽ,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//ʹ�ò�����,   BRT_Timer1,BRT_Timer2
	u32	UART_BaudRate;		//������,       ENABLE,DISABLE
	u8	Morecommunicate;	//���ͨѶ����, ENABLE,DISABLE
	u8	UART_RxEnable;		//�������,   ENABLE,DISABLE
	u8	BaudRateDouble;		//�����ʼӱ�, ENABLE,DISABLE
	u8	UART_Interrupt;		//�жϿ���,   ENABLE,DISABLE
	u8	UART_Polity;		//���ȼ�,     PolityLow,PolityHigh
	u8	UART_P_SW;			//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	u8	UART_RXD_TXD_Short;	//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE

} COMx_InitDefine; 

extern	COMx_Define	COM1,COM2;

u8	USART_Configuration(u8 UARTx);
void SendData(u8 UARTx,u8 dat);	//д�뷢�ͻ��壬ָ��+1
void SendString(u8 UARTx,u8 *puts);
void ResetTxBuffer(u8 UARTx);

void PrintString2(unsigned char *str);
void PrintString1(unsigned char *str);

//void SendDebug(u8* format,...);//��ӡ���ڵ�����Ϣ
void reset_uart1_rx_buff(void);
u8 get_uart1_rx_buff_len(void);
u8 get_uart1_buff(u8 i);


#endif

