#ifndef  _MODBUS_LIB_H_
#define  _MODBUS_LIB_H_

#include "config.h"

unsigned int gen_crc_16(unsigned char *ptr,  unsigned char len);
int create_03_pkg(u8* buff,u8 addr,u16 start,u16 lens,u8* p_pkg_len);
int read_03_response_pkg(u8* buff,u8 addr, u8 n);

#endif