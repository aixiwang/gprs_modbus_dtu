
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


#ifndef		__CONFIG_H
#define		__CONFIG_H


/*********************************************************/

#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟


/*********************************************************/

#include   "stdio.h"
#include   "stdarg.h"
#include   "string.h"

#include  "stc15fxxxx.h"  
#include "delay.h"
#include "usart.h"
#include "led_rgb.h"
#include "st7567_lcd.h"
#include "eeprom.h"
#include "device_ctrl.h"
#include "a6_module.h"

#include "utils_lib.h"
#include "modbus_lib.h"
#include "gprs_lib.h"


#endif
