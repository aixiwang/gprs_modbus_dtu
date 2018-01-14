
#include "config.h"
#include "delay.h"
#include "key_board.h"
#include "string.h"

//º¸≈Ã∂®“Â

sbit  KB_R3 =P0^3;
sbit  KB_R2=P0^2;
sbit  KB_R1=P0^1;
sbit  KB_R0=P0^0;

sbit  KB_L3=P2^7;
sbit  KB_L2=P2^6;
sbit  KB_L1=P2^5;
sbit  KB_L0=P2^4;

 

u8 xdata kb_board[4][4]=
 {{KB_VAL_0,KB_VAL_1,KB_VAL_2,KB_VAL_3},	 //S1-S4
  {KB_VAL_4,KB_VAL_5,KB_VAL_6,KB_VAL_7},	//S5-S8
  {KB_VAL_8,KB_VAL_9,KB_VAL_X,KB_VAL_P4},	//S9-S12
  {KB_VAL_DIAL,KB_VAL_SMS,0,0}				//S13,S14 
  };
					
						 
void  key_board_init()
{

//  P3M1 = 0x00;
//	P3M0 = 0xFF;
   
   KB_R3=1;
   KB_R2=1;
   KB_R1=1;
   KB_R0=1;
   
   KB_L3=0;
   KB_L2=0;
   KB_L1=0;
   KB_L0=0;  
          
}

 u8 scan_row(unsigned char highOrlow)	  //1--high,0--low
 {
     u8 row;

   if(KB_R3 == highOrlow)
   {  	 
   	//  SendDebug("row=3\r\n"); 
      row=3u;
   }
   else	if(KB_R2 == highOrlow)
   {
      row=2u;
   }
   else	 if(KB_R1 == highOrlow)
   {  
      row=1u;
   }
   else	 if(KB_R0 == highOrlow)
   { 
      row=0u;
   }
   else
   {    
     row=-1;
   }   

   return row;  
 }
  
u8 scan_kb()
{
   u8 row,line;
   u8 key_val=-1;
   u8 i;  
   
   key_board_init();     
   //delay_ms(1);	
   
   row=scan_row(0u);  
   if(row==(u8)-1)
   { 
 	 return -1;
   }
 
   i=0;
   while(i<=3)
   {
   		switch(i)
   		{
   			case 0:
   				KB_L0=1;
   				break;
   			case 1:
   				KB_L1=1;
   				break;
   			case 2:
   				KB_L2=1;
   				break;
   			case 3:
   				KB_L3=1;
   				break;
   		}
   			   			
   	//	delay_ms(1);	   		 	
		
		if(scan_row(0) == -1)
		{
		   	line=i;	
		   	break;    	
		}
		    
		 i++;
	}
	
	//key_board_init();	

	if(line<=3)	  
	{
		key_val=kb_board[row][line]; 			
	}
		 		
 	return key_val; 		
}

