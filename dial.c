/* $Id: test_c3p2k.c,v 1.2 2006/02/02 11:11:52 jmt Exp $ */

/*
  Test of C3P2K card implementation in LTC3X driver

  Josep Mones i Teixidor
*/

/*LUIS DESPACHO 934137112
  LUIS MOVIL 639381800 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	while (1)
	{
		printf ("atdt 678788174\n\r");
		fflush (stdout);
		sleep (20);
      printf ("\n\r"); 
      sleep (3);
	}
}
