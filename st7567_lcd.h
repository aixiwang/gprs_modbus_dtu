
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


//坐标原点都在屏幕左上角
//cusor manage
typedef struct
{
   int  x; //0-15
   int  y; //0-3
}CURSOR_POSITION;

//屏幕坐标
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

//光标管理
 void reset_cursor();
 void cursorNext();	//光标下一一个位置
 void cursorPre(); //光标前移一个位置
 CURSOR_POSITION getCursorPos();  //得到当前光标位置
 void setCursorPos(CURSOR_POSITION cursor);	 //设置当前光标位置

//在当前光标位置显示，不改变当前光标位置
void displayChar(u8 chr);
void displayString(u8* pString);								

void displayCharWidthScreenPos(u8 page,u8 column,u8 chr);		//在指定屏幕位置显示一个字符（使用屏幕坐标）
void displayStringWidthScreenPos(u8 page,u8 column,u8* str);		//在指定屏幕位置显示一个字符串（使用屏幕坐标）

//光标自动移动到下一个位置
void inputChar(u8 chr);
void inputString(u8* pString);
void inputBackspace();  //输入退格键

//图片显示管理
void display_dial_pic(u8 bOn);

#endif
