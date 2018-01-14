//--------------------------------------
// GPRS_LIB
// Copyright by Aixi Wang
//--------------------------------------

#include "config.h"

char xdata expected_str[32];
char xdata str_temp[128];
char xdata str_temp2[128];
u8 expected_str_flag = 0;

u8 check_index = 0;
u8 check_index_stop = 0;

u32 xdata sensor_data[SENSOR_DATA_LEN];

//***********************************************
//                    H A L 
//***********************************************



//------------------------
// gprs_rx_int
//------------------------
void gprs_rx_int(u8 c)
{

	

	if (expected_str_flag == 1)
		return;

	// uart1_tx(c);

	if (expected_str[check_index] == c)
	{

		check_index++;
		if (check_index >= check_index_stop)
		{
			// uart1_tx('#');
			expected_str_flag = 1;
		} 
	}
	else
	{	
		check_index = 0;
		// uart1_tx('.');
	}


	
}

//***********************************************
//                F U N C T I O N 
//***********************************************
//-------------------------
// hexit
//-------------------------
char hexit(unsigned char value)
{

	char result;
	result = value & 0x0F;
	if ((result >= 0) && (result <= 9))
		return (result + '0');
	else
		return ('A' + (result - 0x0a));
}

//-------------------------
// hexdump
//-------------------------
void hexdump(u8 * p_buff,int n)
{

#if 0
    int i,j;
    u8 c,d;

    for (i = 0; i < n; i++)
    {
            c = p_buff[i];
            d = c & 0x0f;

			uart1_tx(hexit(c>>4));
			uart1_tx(hexit(d));			
			uart1_tx(' ');

    }

   	uart1_tx('#');
	uart1_tx('\r');
	uart1_tx('\n');
#endif

}

//------------------------
// set_expected_str
//------------------------
void set_expected_str(u8* p)
{
	EA = 0;
	strcpy(expected_str,p);
	expected_str_flag = 0;
	check_index = 0;
	check_index_stop = strlen(expected_str);

	EA = 1;
}

//------------------------
// get_expected_str_status
//------------------------
int get_expected_str_status()
{
	return expected_str_flag;
}


//------------------------
// atcmd
//------------------------
int atcmd(u8* at_str,u16 timeout)
{
	u16 i;
	i = timeout*100;
	set_expected_str("OK");
	lcd_putstr("0801");
	uart2_puts(at_str);


	while (i--)
	{
		if (get_expected_str_status() == 1)
		{
			// uart1_tx("#");
			return 0;
		}
		else
		{
			// uart1_tx("*");
			delay_ms(10);
		}
	}
	
	return -1;
}

//------------------------
// atcmd2
//------------------------
int atcmd2(u8* at_str,u8* wait_str,u16 timeout)
{
	u16 i;
	i = timeout*100;
	set_expected_str(wait_str);

	lcd_putstr("0802");
	
	uart2_puts(at_str);


	
	if (strlen(wait_str) == 0){
		delay_ms(timeout*1000);
		return 0;
	}
	while (i--)
	{
		if (get_expected_str_status() == 1)
		{
			// lcd_putstr("@");
			return 0;
		}
		else
		{
			// lcd_putstr("+");
			delay_ms(10);
		}
	}
	
	return -1;	
	
}
	
//------------------------
// grps_att_act
//------------------------
int grps_att_act()
{
	u8 retcode = 0;
	u8 n;

	
	n = 3;
	while (n > 0)
	{
		retcode = atcmd("ATE0\r\n",30);
		if (retcode == 0)
		{	
			break;
		}
		else
		{
			lcd_putstr("F801");
		}
	}
	
	// AT+CGATT
	n = 3;
	while (n > 0)
	{
		retcode = atcmd("AT+CGATT=1\r\n",30);
		if (retcode == 0)
		{	
			break;
		}
		else
		{
			lcd_putstr("F802");
		}
	}

	// AT+CGACT
	n = 3;
	while (n > 0)
	{
		retcode = atcmd("AT+CGACT=1,1\r\n",30);
		if (retcode == 0)
		{	
			break;
		}
		else
		{
			lcd_putstr("F803");
		}
	}


	return 0;
	
}


//------------------------
// tcp_connect
//------------------------
int tcp_connect(u8* ip,u16 port)
{
	u8 d[6];
	strcpy(str_temp,"AT+CIPSTART=\"TCP\",\"");
	strcat(str_temp,ip);
	strcat(str_temp,"\",");
	sprintf(d,"%d",port);
	strcat(str_temp,d);
	strcat(str_temp,"\r\n");
	return atcmd2(str_temp,"OK",30);	
	
}


//------------------------
// tcp_send
//------------------------
int tcp_send(u8* s,u16 str_len)
{
	u8 retcode = 0;
	u8 d[6];
	strcpy(str_temp,"AT+CIPSEND=");
	sprintf(d,"%d",str_len);
	strcat(str_temp,d);
	strcat(str_temp,"\r\n");
	
  retcode = atcmd2(str_temp,">",10);
  retcode = atcmd2(s,"OK",10);
  // retcode += atcmd2("\r\n","",3);
  // retcode += atcmd("AT",10);
  if (retcode == 0)
	return 0;
  else
	return -1;
	
}


//------------------------
// tcp_close
//------------------------
int tcp_close()
{
	return atcmd("AT+CIPCLOSE\r\n",30);
}

//------------------------
// get_csq
//------------------------
int get_csq(u8* d)
{
#if 0
	        retcode, s = self.gprs_at.atcmd3('AT+CSQ\r\n','OK\r\n',5)
        
        if retcode < 0:
            return -1,0
            
        i = s.find('+CSQ: ')
        if  i >= 0:
            j = s.find(',')
            #print 'i:',i,'j:',j
            if j > 0:
                csq_val = int(s[i+5:j])
                
                return 0, csq_val
            else:
                return -1,0
        else:
            return -2, 0
#endif
	return -1;
}

//------------------------
// create_data_pkg
//------------------------
int create_data_pkg(u8* smd_id,u32* p,u8 n,u8* p_desc)
{
	int i,j;
	char xdata data_array_str[64];
	
	if (n >= 8)
		return -1;
	
	j = 0;
	for (i=0; i<n; i++)
	{
		if (i == (n-1))
			sprintf(data_array_str + j,"%ld",*(p+i));
		else
			sprintf(data_array_str + j,"%ld,",*(p+i));
		j = strlen(data_array_str);
	}	
	sprintf(p_desc,"{\"msg_id\":\"1\",\"smd_id\":\"%s\",\"data\":[%s]}",smd_id,data_array_str);
		
	return 0;
}	



//---------------------------
// gprs_send
//---------------------------        
int gprs_send(u8* ip,u16 port,u8* smd_id,u32* p,u8 n)
{
	int retcode;
	int n2;

	char data_header[10];

	retcode = grps_att_act();

	if (retcode < 0)
	{	
		lcd_putstr("F804");
		return retcode;
	}

   #if 0
	retcode = tcp_close();

	if (retcode < 0)
	{	
		lcd_putstr("F805");
		return retcode;
	}
   #endif

	retcode = tcp_connect(ip,port);

	if (retcode < 0)
	{	
		lcd_putstr("F806");
		return retcode;
	}


	create_data_pkg(smd_id,p,n,str_temp2);
	n2 = strlen(str_temp2);
	sprintf(data_header,"*%d\r\n",n2);


	strcpy(str_temp,data_header);
	strcat(str_temp,str_temp2);
	strcpy(str_temp2,str_temp);

	retcode = tcp_send(str_temp2,strlen(str_temp2));
	if (retcode < 0)
	{	
		lcd_putstr("F807");
		return retcode;
	}


	retcode = tcp_close();
	if (retcode < 0)
	{	
		lcd_putstr("F806\r\n");
		return retcode;
	} 


	retcode = atcmd("AT\r\n",5);
	if (retcode < 0)
	{	
		lcd_putstr("F808");
		return retcode;
	} 
	return 0;	
}	