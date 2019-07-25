#include <hidef.h>
#include <rs232.h>

#pragma CREATE_ASM_LISTING ON
/* terminal.wnd */
#define NOCHAR  0

#include <stdlib.h>
#include <hidef.h>
#include <ctype.h>
#include "rs232.h"

#pragma DATA_SEG ram1
unsigned char buff[BUFFER_SIZE];


#pragma DATA_SEG RAM2   //Segmento RAM en 100
char rs_next_in=0;
char rs_next_out=0;

char RS_kbhit()
{
	return (rs_next_in!=rs_next_out);
}
void RS_Init(unsigned char baud, unsigned char options)
{
  BAUD  = baud;     /* baud rate 9600 at 8 MHz */
  SCCR1 = 0;
  SCCR2 = options;     /* 8 bit, TE and RE set */   

  rs_next_in=0;
  rs_next_out=0;
}

unsigned char RS_GetChar(void) 
{
	 char c;

	c=buff[rs_next_out];
	rs_next_out++;
//	if (rs_next_out>=BUFFER_SIZE-1)
//		rs_next_out=0;
	rs_next_out&=BUFFER_SIZE;

	return c;
}

void RS_Enable(char enable)
{
	unsigned int n;
	while (!(SCSR & 0x80));  /* wait for output buffer empty */   
	while (!(SCSR & 0x40));  /* wait for output register empty */   

	RS485_ENABLE = enable;
}


void RS_PutString (char *str, char len)
{
	char n;
	
	for (n=0;n<len ;n++ )
		RS_PutChar (str[n]);
}

void RS_PutChar(char ch) {
  /* sends a character to the terminal channel */
	while (!(SCSR & 0x40));  /* wait for output register empty */   
  SCDAT = ch;   
}

void RS_isr ()
{
	char t;
	while (!(SCSR & 0x20)); /* wait for input */   

	
	buff[rs_next_in]=SCDAT;
	t=rs_next_in;
		
	rs_next_in++;
	
	rs_next_in&=BUFFER_SIZE;

//    if(rs_next_in==rs_next_out)
//		rs_next_in=t;           // Buffer full !!
}
