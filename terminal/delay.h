#ifndef __DELAY_H
#define __DELAY_H
/*	C6805 Code Development System
	Delay library for Motorola 68HC05
		DELAY.H
 	This code may be adapted for any purpose when
	used with the C6805 Code Development System.  No
	warranty is implied or given as to their usability
	for any purpose.

	(c) Copyright 1998 Byte Craft Limited
	421 King St.N., Waterloo, ON, Canada, N2J 4E4
	VOICE: 1 (519) 888 6911
	FAX  : 1 (519) 746 6751
	email: support@bytecraft.com
	Andre' Labelle
REVISION HISTORY
V1.00 AL 02/98 initial version
*/

#ifndef OSC
#define OSC 800 /* default oscillator frequency of 4Mhz */
#endif

#if (OSC<=5600)
#define DELAY_MIN 20 // 20khz or 50us
#define delay_100us() elapse(SPEED);
#define delay_ms(arg) delay_loop_10(arg);

#else
#if (OSC%200)
#error Delay Library <Invalid OSC value>: OSC must be a multiple of 200khz
#endif

#define DELAY_MIN 40 //40khz or 25us
#define delay_100us() elapse(SPEED); elapse(SPEED); NOP(); 
#define delay_ms(arg) delay_loop_20(arg);
#endif

#define SPEED (OSC/DELAY_MIN)


#ifndef BRN
#define BRN #asm( dw 0x2100 )  //Branch never (a three cycle NOP)
#endif

void elapse(char);
void delay_loop_10(unsigned char t);
void delay_loop_20(unsigned char t);

#endif /* __DELAY_H */
