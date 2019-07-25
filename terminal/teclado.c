#include <stdlib.h>
#include <hidef.h>
#include <ctype.h>
#include "term.h"
#include "delay.h"
#include "dmf5001n.h"
#include "teclado.h"

const char scan[]={'`', 0x0E,
			  '\x8',0x66,
			  '\t',0x0d,
			  '1', 0x16,
			  '2', 0x1E,
			  '3', 0x26,
			  '4', 0x25,
			  '5', 0x2E,
			  '6', 0x36,
			  '7', 0x3D,
			  '8', 0x3E,
			  '9', 0x46,
			  '0', 0x45,
			  '-', 0x4E,
			  '=', 0x55,
			  'q', 0x15,
			  'w', 0x1D,
			  'e', 0x24,
			  'r', 0x2D,
			  't', 0x2C,
			  'y', 0x35,
			  'u', 0x3C,
			  'i', 0x43,
			  'o', 0x44,
			  'p', 0x4D,
			  '[', 0x54,
			  ']', 0x5B,
			  '\\',0x5D,
			  'a', 0x1C,         
			  's', 0x1B,
			  'd', 0x23,
			  'f', 0x2B,
			  'g', 0x34,
			  'h', 0x33,
			  'j', 0x3B,
			  'k', 0x42,
			  'l', 0x4B,
			  ';', 0x4C,
			  'z', 0x1A,
			  'x', 0x22,
			  'c', 0x21,
			  'v', 0x2A,
			  'b', 0x32,
			  'n', 0x31,
			  'm', 0x3A,
			  ',', 0x41,
			  '.', 0x49,
			  '/', 0x4A,
			  ' ', 0x29,
			  '-', 0x7B,
			  '/', 0xE0,
			  '.', 0x71,
			  '*', 0x7C,
			  '+', 0x79,
			  '0', 0x70,
			  '1', 0x69,
			  '2', 0x72,
			  '3', 0x7A,
			  '4', 0x6B,
			  '5', 0x73,
			  '6', 0x74,
			  '7', 0x6C,
			  '8', 0x75,
			  '9', 0x7D,
			  0xd, 0x5a,
			  0xf0,0xf0,
			  0xff,0xff};

unsigned char key_kbhit()
{
	if (!CLOCK && !DATA)
		return TRUE;
	else
		return FALSE;
}

unsigned char key_read (void)
{
	unsigned char n,idx;
	unsigned char dt=0;

	for (n=0;n<8;n++ )
	{
		while (!CLOCK);
		while (CLOCK);

		dt=(dt>>1);

		if (DATA)
			dt|=0x80;
	}

	for (idx=0;scan[idx+1]!=0xff && scan[idx+1]!=dt;idx+=2 );
	return scan[idx];
}