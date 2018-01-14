
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


#ifndef		__ST7567_LCD_H
#define		__ST7567_LCD_H


//����ԭ�㶼����Ļ���Ͻ�
//cusor manage
typedef struct
{
   int  x; //0-15
   int  y; //0-3
}CURSOR_POSITION;

//��Ļ����
typedef struct
{
   int  column; //0-127
   int  page; //0--7   
}SCREEN_POSITION;


void Init_ST7567_LCD();

void Write_ST7567_DISPLAY_DATA_serial_4SPI(unsigned char DADA_4SPI);
void Write_ST7567_COM(unsigned char COMDADA_4SPI);
void HDReset();
void clear_screen(unsigned char xx);

//������
 void reset_cursor();
 void cursorNext();	//�����һһ��λ��
 void cursorPre(); //���ǰ��һ��λ��
 CURSOR_POSITION getCursorPos();  //�õ���ǰ���λ��
 void setCursorPos(CURSOR_POSITION cursor);	 //���õ�ǰ���λ��

//�ڵ�ǰ���λ����ʾ�����ı䵱ǰ���λ��
void displayChar(u8 chr);
void displayString(u8* pString);								

void displayCharWidthScreenPos(u8 page,u8 column,u8 chr);		//��ָ����Ļλ����ʾһ���ַ���ʹ����Ļ���꣩
void displayStringWidthScreenPos(u8 page,u8 column,u8* str);		//��ָ����Ļλ����ʾһ���ַ�����ʹ����Ļ���꣩

//����Զ��ƶ�����һ��λ��
void inputChar(u8 chr);
void inputString(u8* pString);
void inputBackspace();  //�����˸��

//ͼƬ��ʾ����
void display_dial_pic(u8 bOn);

#endif
