#include <stdlib.h>
#include <hidef.h>
#include <ctype.h>
#include "term.h"
#include "delay.h"
#include "dmf5001n.h"

#define CMD_AUTO_WRITE					0xb0
#define CMD_AUTORESET					0xb2
#define CMD_TEXT_HOME_ADDRESS_SET		0x40
#define CMD_TEXT_AREA_SET				0x41
#define CMD_GRAPHIC_HOME_ADDRESS_SET	0x42
#define CMD_GRAPHIC_AREA_SET			0x43
#define	CMD_CURSOR_POINTER_SET			0X21
#define	CMD_ADDRESS_POINTER_SET			0x24
#define CMD_SCREEN_PARKING				0xe0
#define CMD_SCREEN_COPY					0xe8
#define CMD_DATA_AUTO_READ				0xb1
#define CMD_DISPLAY_MODE				0X90

union
{
   unsigned char valor;
   struct
   {
       unsigned int _beep	:1;    //Bit 0
	   unsigned int _light	:1;    //Bit 1
	   unsigned int _wr	    :1;    //Bit 2
	   unsigned int _rd		:1;    //Bit 3
       unsigned int _ce		:1;    //Bit 4
       unsigned int _cd     :1;    //Bit 5
       unsigned int _halt	:1;	   //Bit 6
       unsigned int _reset	:1;    //Bit 7
   }_bits;
}CmdDisp;

union
{
	unsigned char valor;
	struct
	{
		unsigned int _CursorBlink	:1;
		unsigned int _ShowCursor	:1;
		unsigned int _TxtDisplayOn	:1;
		unsigned int _GrDisplayOn	:1;	//Bit 0
		unsigned int _Addrs0		:1;
		unsigned int _Addrs1		:1;
		unsigned int _Addrs2		:1;
		unsigned int _Addrs3		:1; //Bit 7
	}_bits;
}DisplayMode;

union
{
	unsigned char valor;
	struct
	{
		unsigned int _AutoScrollOff	:1;	//Bit 0
		unsigned int _LocalEdit		:1;
	}_bits;
}settings;

unsigned char cursorX, cursorY;

//------------------ Comprueba que el display est‚ libre ---------------------
void busy(unsigned mask)
{
	DDRB=0;	//Port B como entradas

	CmdDisp._bits._ce = FALSE;
	CmdDisp._bits._rd = FALSE;
	PORTA=CmdDisp.valor;

	while ((PORTB&mask)!=mask);	//Esperamos a que esté libre

	CmdDisp.valor|=0xfc;
	PORTA = CmdDisp.valor;
}

unsigned char RdStatus ()
{
	unsigned char status;

	DDRB=0;	//Port B como entradas

	CmdDisp._bits._ce = FALSE;
	CmdDisp._bits._rd = FALSE;
	PORTA=CmdDisp.valor;

	status=PORTB;

	CmdDisp.valor|=0xfc;
	PORTA = CmdDisp.valor;
	return status;
}

//---------------- Transmite al display un comando ---------------------------
void WrtCmd (unsigned char cmd)
{
	busy(0x3);
	DDRB=0xff;

	CmdDisp._bits._ce =0;
	CmdDisp._bits._wr =0;
	PORTA = CmdDisp.valor;
	PORTB = cmd;
	CmdDisp.valor |= 0xfc;
	PORTA = CmdDisp.valor;
}

//---------------- Transmite al display un dato -----------------------------
void WrtData (unsigned char data)
{

	busy(0x3);
	DDRB = 0xff;	//PORTB como salidas

	CmdDisp._bits._cd=FALSE;
	CmdDisp._bits._ce = FALSE;
	CmdDisp._bits._wr = FALSE;
	PORTA = CmdDisp.valor;
	PORTB = data;

	CmdDisp.valor|=0xfc;
	PORTA = CmdDisp.valor;
}
//---------------- Lee del display un dato -----------------------------
unsigned char RdData ()
{
	unsigned char data;

	DDRB = 0;	//PORTB como entradas

	CmdDisp._bits._ce = FALSE;
	CmdDisp._bits._rd = FALSE;
	CmdDisp._bits._cd = FALSE;
	PORTA = CmdDisp.valor;

	data = PORTB;
	CmdDisp.valor|=0xfc;
	PORTA = CmdDisp.valor;
	
	return data;
}

void DisplayBeep (unsigned char time)
{
	CmdDisp._bits._beep=TRUE;
	PORTA= CmdDisp.valor;

	delay_ms (time);
	CmdDisp._bits._beep=FALSE;
	PORTA= CmdDisp.valor;
}

void DisplayLight (unsigned char light)
{
	CmdDisp._bits._light=light;
	PORTA= CmdDisp.valor;
}

//------------- POSICIONAMOS EL POINTER MEMORY --------------------------
void DisplayPosMem (unsigned int addrs)
{
	WrtData (LOBYTE (addrs));
	WrtData (HIBYTE (addrs));
	WrtCmd (CMD_ADDRESS_POINTER_SET);	//Address point set
}

void DisplayGotoXY (unsigned char x, unsigned char y)
{
	DisplayPosMem ((32*y)+x);

	WrtData (x);
	WrtData (y);
	WrtCmd (CMD_CURSOR_POINTER_SET);
	cursorX=x;
	cursorY=y;
}

void DisplayCursorPattern (unsigned char pattern)
{
	WrtCmd (pattern|0xa0);
}

void DisplayCursor (unsigned char on)
{
	if (!on)
		DisplayMode._bits._ShowCursor = FALSE;
	else
		DisplayMode._bits._ShowCursor = TRUE;

	WrtCmd (DisplayMode.valor);
}

void DisplayLuz (char luz)
{
	CmdDisp._bits._light = luz;
	PORTA = CmdDisp.valor;
}

void DisplayTextClear(void)
{
	unsigned int n;

	DisplayPosMem(0);
	WrtCmd (CMD_AUTO_WRITE);
	
	for (n=0;n<1024;n++)
	{
//		busy(0x8);
		WrtData (0);
	}

	WrtCmd (CMD_AUTORESET);

	DisplayGotoXY (0,0);
}

// Inicializa el display de la siguiente manera:
//
// - Logically "AND" of Graphic and Text Display
// - Cursor blink ON
// - Cursor display ON
// - Text display ON
// - CG ROM mode
// - 8 line width cursor
// - Text display 2480x64 DOT (fuentes de 8x8)
// - Graphic display 240x64 DOT
void DisplayInit (void)
{
	unsigned int n;

	DisplayMode.valor = CMD_DISPLAY_MODE;

	CmdDisp.valor=0xfc;
	PORTA=CmdDisp.valor;
	delay_ms (100);

//    lineaH=0x82;

	CmdDisp._bits._reset=FALSE;
	PORTA=CmdDisp.valor;
	delay_ms(1);
	CmdDisp._bits._reset=TRUE;
	PORTA=CmdDisp.valor;

	WrtCmd (0x80);	//Text Only Mode

	WrtData (0);
	WrtData (0);
	WrtCmd (CMD_TEXT_HOME_ADDRESS_SET);	

	WrtData (0x20);
	WrtData (0x0); 
	WrtCmd (CMD_TEXT_AREA_SET); //Number of Text area Command.

	
	WrtData (0);
	WrtData (0);
	WrtCmd (CMD_GRAPHIC_HOME_ADDRESS_SET);		//Graphics home area set

	WrtData (0x20);
	WrtData (0);
	WrtCmd (CMD_GRAPHIC_AREA_SET);

	DisplayPosMem (0x100);
	WrtCmd (CMD_AUTO_WRITE);
		
	for (n=0;n<1024;n++ )
		WrtData ('H');

	WrtCmd (CMD_AUTORESET);

	DisplayTextClear();
	DisplayGotoXY (0,0);

	DisplayCursor(TRUE);
	DisplayCursorPattern (7);

	DisplayMode._bits._TxtDisplayOn=TRUE;
	DisplayMode._bits._ShowCursor=TRUE;
	DisplayMode._bits._CursorBlink=TRUE;
	WrtCmd (DisplayMode.valor);

	cursorX =0;
	cursorY =0;
}

void CursorAddX()
{
	cursorX++;
	if (cursorX>32)
	{
		cursorX=0;
		cursorY++;
		DisplayGotoXY (cursorX,cursorY);
			if (cursorY>15)
			DisplayScroll ();
	}
}

void DisplayWrite (unsigned char *buffer)
{
	static char flag=0;
	unsigned char n,x;

	WrtCmd (CMD_AUTO_WRITE);

	for (n=0;buffer[n]!=0 ;n++)
	{
		if (flag)
		{
			if (flag==1)
			{
				flag=0;
				switch (buffer[n])
				{
					case '\x7d':
						WrtCmd (CMD_AUTORESET);
						DisplayGotoXY (0,0);
						WrtCmd (CMD_AUTO_WRITE);
					break;

					case '*':
					case ',':
					case ':':
					case ';':
					case '+':
						DisplayTextClear();
					break;

					case ''':
						flag=2;
					break;

					case 'T':
						for (x=0;x<32-cursorX ;x++)
							WrtData (0);

						WrtCmd (CMD_AUTORESET);
						DisplayGotoXY (cursorX,cursorY);
						WrtCmd (CMD_AUTO_WRITE);
					break;

					case 'a':
						while (TRUE);
					break;
				}
			}
			else if (flag==2)
			{
				flag=0;
				switch (buffer[n])
				{
					case '0':
						WrtCmd (CMD_AUTORESET);
						DisplayCursor(FALSE);
						WrtCmd (CMD_AUTO_WRITE);
					break;

					case '1':
						WrtCmd (CMD_AUTORESET);
						DisplayCursor(TRUE);
						WrtCmd (CMD_AUTO_WRITE);
					break;
				}
			}
		}
		else
		{
			switch (buffer[n])
			{
				case '\x7':	//Ctrl-G
					DisplayBeep(500);
				break;

				case '\x8': //Back Space
					if (cursorX)
					{
						cursorX--;
						WrtCmd (CMD_AUTORESET);
						DisplayGotoXY (cursorX,cursorY);
						WrtCmd (CMD_AUTO_WRITE);
					}
				break;

				case '\x9': //Tab
					for (x=0;x<3 ;x++ )
					{
						busy(0x8);
						WrtData (0);

						cursorX++;
						if (cursorX>32)
						{
							WrtCmd (CMD_AUTORESET);
							cursorX=0;
							cursorY++;
							DisplayGotoXY (cursorX,cursorY);
								if (cursorY>15)
								DisplayScroll ();
							WrtCmd (CMD_AUTO_WRITE);
						}
					}
				break;

				//Ctrl-K 
				//Mueve el cursor una linea arriba
				case '\xb': 
					WrtCmd (CMD_AUTORESET);
					cursorY--;
					if (!cursorY)
						cursorY=15;
					DisplayGotoXY (cursorX,cursorY);
					WrtCmd (CMD_AUTO_WRITE);
				break;

				//Ctrl-[ o ESC
				//Inicia la secuencia de escape
				case '\x1b':
					flag=TRUE;
				break;
				
				case '\n':  //Line Feed
					WrtCmd (CMD_AUTORESET);
					cursorY++;
					DisplayGotoXY (cursorX,cursorY);
					if (cursorY>15)
						DisplayScroll ();
					WrtCmd (CMD_AUTO_WRITE);
				break;

				case '\r': //Carriage Return
					WrtCmd (CMD_AUTORESET);
					cursorX=0;
					DisplayGotoXY (cursorX,cursorY);
					WrtCmd (CMD_AUTO_WRITE);
				break;

				//Ctrl-V
				//Mueve el cursor abajo sin scroll
				case '\x16': 
					WrtCmd (CMD_AUTORESET);
					cursorY++;
					if (cursorX>32)
						cursorY=32;
					DisplayGotoXY (cursorX,cursorY);
					WrtCmd (CMD_AUTO_WRITE);

				break;

				//Ctrl-Z
				//Borra la pantalla
				case '\x1a':
					DisplayTextClear();
				break;
				
				//Ctrl-^
				//Home
				case '\x1e':
					WrtCmd (CMD_AUTORESET);
					DisplayGotoXY (0,0);
					WrtCmd (CMD_AUTO_WRITE);
				break;

				//Ctrl-_
				//Avanza el cursor una columna
				case '\x1f':
					WrtCmd (CMD_AUTORESET);
					CursorAddX();
					WrtCmd (CMD_AUTO_WRITE);
				break;
				
				default:
					busy(0x8);
					WrtData (buffer[n]-' ');
					WrtCmd (CMD_AUTORESET);
					CursorAddX();
					WrtCmd (CMD_AUTO_WRITE);

				break;
			}
		}
	}
	
	WrtCmd (CMD_AUTORESET);
	DisplayGotoXY (cursorX,cursorY);
}

void DisplayWriteb (unsigned char bt)
{
	static unsigned char mem[2];

	mem[0]=bt;
	mem[1]=0;
	DisplayWrite (mem);
}


void DisplayScroll ()
{
	unsigned char mem[34];
	unsigned char x,y;
	unsigned char antCurX=cursorX;
	
	for (y=0;y<15 ;y++)
	{
		DisplayPosMem ((y+1)*32);

		WrtCmd (CMD_DATA_AUTO_READ);
			for (x=0;x<32 ;x++)
			{
//				busy(0x2);
				mem[x]=RdData ();
			}
		WrtCmd (CMD_AUTORESET);
		
		DisplayPosMem (y*32);
		WrtCmd (CMD_AUTO_WRITE);		
			for (x=0;x<32 ;x++)
			{
//				busy(0x8);
				WrtData (mem[x]);
			}
		WrtCmd (CMD_AUTORESET);
	}

	DisplayPosMem (15*32);
	WrtCmd (CMD_AUTO_WRITE);		
		for (x=0;x<32 ;x++)
		{
//			busy(0x8);
			WrtData (0);
		}
	WrtCmd (CMD_AUTORESET);
	DisplayGotoXY (antCurX,15);
}
