



#ifndef  _KEY_BOARD_H_
#define  _KEY_BOARD_H_

typedef enum
{
	 KB_VAL_NULL=-1,
	 KB_VAL_0='0',//S1
	 KB_VAL_SMS='S',//S2,sms
	 KB_VAL_8='8',//S3
	 KB_VAL_4='4',//S4
	 KB_VAL_2='2',//S5
	 KB_VAL_6='6',//S6
	 KB_VAL_5='5',//S7
	 KB_VAL_1='1',//S8
	 KB_VAL_3='3',//S9
	 KB_VAL_7='7',//S10
	 KB_VAL_9='9',//S11
	 KB_VAL_X='*',//S12	  *
	 KB_VAL_DIAL='D',//S13
	 KB_VAL_P4='#'//S14	 plus 4	 #
};


void  key_board_init();
u8 scan_kb();

#endif