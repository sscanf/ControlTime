/*	C6805 Code Development System
	Delay library for Motorola 68HC05
		DELAY.C
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
#pragma NO_OPTIMIZE

#define nop asm nop

#include "delay.h"


/*	delay_ms() is defined as delay_loop_20()
	if the oscillator frequency is > 5.6Mhz
	delay_loop_20() calls elapse 20 times for every t
	elpase() will elapse 50us when passed
	SPEED with an offset (compensation for call or looping overhead)
*/

void delay_loop_20(unsigned char t)
   {
   int delay_tmp=19;
NO_OPTIMIZE_20:  //Make sure this next statement does not get optimized
   elapse(SPEED-25-15);	//-25 for call and return overhead
asm D1ms:                  	//-15 for elapse overhead
   elapse(SPEED-10-15);	//-10 for looping overhead
   nop;
   asm
	{
      DEC delay_tmp
      BNE D1ms
      LDA #20
      STA delay_tmp
	}
   nop;
   nop;
   nop;
   asm
   {
      DEC t
      BNE D1ms
   }
 }


/* 	delay_ms() is defined as delay_loop_10()
	if the oscillator frequency is <= 5.6Mhz
	delay_loop_10() calls elapse 10 times for every t
	elpase() will elapse 100us when passed
	SPEED with an offset (compensation for call or looping overhead)
*/

void delay_loop_10(unsigned char t)
   {
   int delay_tmp=9;
NO_OPTIMIZE_10:  //Make sure this next statement does not get optimized
   elapse(SPEED-25-15);	//-25 for call and return overhead
asm D500us:                	//-15 for elapse overhead
   elapse(SPEED-15-15);	//-15 for looping overhead
   nop;
   nop;
   nop;
   asm {
      DEC delay_tmp
      BNE D500us
   }
   nop;
   nop;
   nop;
   asm{
      LDA #10
      STA delay_tmp
      DEC t
      BNE D500us
   }
   }

/*	elapse was created for use with the delayXXX functions
	Tcyc=tic+9
	this function will elapse tic+9 internal clock cycles
	tic%5=0
	tic must be a multiple of 5
*/

void elapse(char tic)
{
   asm{
             	// OSC	Tcyc's
TIC:  SUB #5 	// ----------------
      BHI TIC	// 1Mhz	  500
             	// 2Mhz	 1000
             	// 8Mhz	 4000
   }
}

