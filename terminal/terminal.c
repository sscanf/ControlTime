//4860
#include <stdlib.h>
#include <hidef.h>
#include <ctype.h>
#include "DMF5001N.H"
#include "term.h"
#include "delay.h"
#include "rs232.h"
#include "teclado.h"

#pragma LOOP_UNROLL

#pragma DATA_SEG SHORT  DS_RegsHC05
TS_RegsHC05 RegsHC05;

#pragma DATA_SEG DEFAULT

#define FRCT (*((unsigned char*)(0x19)))
#define CCR (*((unsigned char*)(0x3fdf)))
extern char _SEX[4], _LEX[4], _JSR[3];

#define CLI()	asm cli
#define SEI()	asm sei

#define FALSE 0	
#define TRUE  1
#define BOOL  unsigned int
#define ULONG unsigned long

/*#pragma DATA_SEG ram1
unsigned char RsBuffer[170];
#pragma DATA_SEG ram1   //Segmento RAM en 100*/


void main (void)
{
	char antPTC=0;	
	char antMUTE=0;
	char n,spc;

   CCR = (unsigned char)0xc0;

	SEI();

	asm
	{
		CLRA
		CLRX
	INIT:
		STA $0x30,X
		INCX
		CPX #0x8f
		BNE INIT
	}

	asm
	{
		CLRA
		CLRX
	INIT2:
		STA $0x100,X
		INCX
		CPX #0x5F
		BNE INIT2
	}
	//Esto est�extraido del m�ulo que STARTUPX.C que viene con el compilador
	//Inicializa el entorno para que el C funcione correctamente.
	 _LEX[0]= lda; _LEX[3]= rts; _SEX[0]= sta; _SEX[3]= rts;
	 _JSR[0]= jmp;  


// Configuraci� de los puertos
	DDRC = 0xf0;
	DDRA = 0xff;
	DDRB = 0xff;
	DDRD = 0x00;


	b_COPF = FALSE;
	b_CME  = FALSE;
	b_COPE = FALSE;
	b_CM1  = 0;
	b_CM0  = 0;
	b_TOIE = FALSE;        //Timer Overflow Interrupt Disable

	PORTC = 0x00;
	PORTA = 0xfc;
	PORTB = 0X00;

	RS_Enable (TRUE);   
	RS_Init(0x30,0x2c);  //01 = 1200,n,8,1


	DisplayInit();

	DisplayLight (FALSE);
	DisplayBeep(100);

	DisplayGotoXY (0,0);

	CLI();
	for (; ; )
	{
		if (RS_kbhit())	//Entra algo por el puerto RS
		{
			spc=RS_GetChar();

/*			if (spc=='\n')
			{
				DisplayWrite ("\n\r");
			}
			else*/
				DisplayWriteb (spc);
		}

		if (key_kbhit())
		{
			spc=key_read();

//			if (spc=='\r')
//				RS_PutString ("\n\r",2);

			if (spc==0xf0)
				spc=key_read();
			else
				RS_PutChar (spc);
		}

/*		for (spc=0;spc<31;spc++ )
		{
			for (n=0;n<spc ;n++ )
				DisplayWrite (" ");
			DisplayWrite ("*\n\r");
		}

		for (spc=30;spc>0;spc-- )
		{
			for (n=0;n<spc ;n++ )
				DisplayWrite (" ");
			DisplayWrite ("*\n\r");
		}*/

	}
}


////////////////////////// COMUNICACIONES ///////////////////////////////////

char SendByteSPI (unsigned char byte)
{
   SPDR = byte;
   while (!b_SPIF);     //Transmite el byte
   return FALSE;
}

char SendWordSPI (unsigned long byte)
{
   SPDR = HIBYTE(byte);
   while (!b_SPIF);     //Transmite el byte

   SPDR = LOBYTE(byte);
   while (!b_SPIF);     //Transmite el byte

   return FALSE;
}



// ----------------------- RUTINAS INTERRUPCION --------------------------

interrupt void SPI()
{
}

interrupt void TIMER()
{
   asm LDA 0x18;
   asm LDA 0x19;
}

interrupt void IRQ()
{
}

interrupt  void SWI()
{
}

interrupt void SCI()
{
	RS_isr();
}
