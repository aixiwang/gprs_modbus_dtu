#ifndef  _GPRS_LIB_H_
#define  _GPRS_LIB_H_

#include "config.h"

#define SENSOR_DATA_LEN 4


void gprs_rx_int(u8 c);
int gprs_send(u8* ip,u16 port,u8* smd_id,u32* p,u8 n);
int get_expected_str_status();

void uart1_tx(u8 c);
void uart2_tx(u8 c);
void uart1_puts(u8 *s);
void uart2_puts(u8 *s);

void hexdump(u8 * p_buff,int n);
void lcd_clr(void);
void lcd_putstr(u8* pstr);
void lcd_putstr2(u8 x,u8 y,u8* pstr);
void lcd_puthex(u8* pstr,u8 n);
char hexit(unsigned char value);

#endif