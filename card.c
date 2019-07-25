#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winscard.h>
#include <pcsclite.h>
#include "reader.h"
#include "timectrl.h"

#define MAKEWORD(low,hi)	((WORD)(((BYTE)(low))|(((WORD)((BYTE)(hi)))<<8)))

SCARDCONTEXT context;
SCARDHANDLE card;
unsigned long dwState,dwProtocol,dwAtrLen;
char *mszReaders;
long rv,hrSts;
DWORD pref;

long waitForCard (int state);

#define CHECK_ERROR_CODE(function,rv,expected) \
{                     \
  if ((LONG)(rv)!=(LONG)(expected)) { \
    ClrScr();\
	printf("ERROR: \"%s\" returned 0x%lX and we expected 0x%lX\n", function, (LONG)(rv), (LONG)(expected)); \
	do{	rv = waitForCard(SCARD_STATE_PRESENT);}while(rv&SCARD_STATE_PRESENT);\
    return (EXIT_FAILURE); \
  } \
}

typedef struct {
  const char *comment;
  const unsigned char *tx;
  int txlen;
  int check_answer;
  const unsigned char *erx;
  int erxlen;
} test_command;


void dump_buffer(const unsigned char *buffer, int length)
{
  char tmp[80];
  char tmp2[80];
  char ascii[20];
  int pos=0, ii;
  
  while(pos<length) {
    sprintf(tmp, "%04X: ", pos);
    for (ii=0; ii<16; ii++, pos++) {
      if (pos<length) {
	sprintf(tmp2, "%02X ", buffer[pos]);
	strcat(tmp, tmp2);
	if (buffer[pos] < 32)
	  ascii[ii] = '.';
	else
	  ascii[ii] = (char) buffer[pos];
      } else {
	strcat(tmp, "   ");
	ascii[ii] = ' ';
      }
    }
    ascii[16] = '\0';
    printf("%s [%s]\n", tmp, ascii);
  }
}

int transmit(SCARDHANDLE card, unsigned char *buffer, DWORD length, const test_command *tc)
{
  LONG rv;

//  printf("%s... ", tc->comment);
  fflush(stdout);
  rv = SCardTransmit(card, SCARD_PCI_T0, tc->tx, tc->txlen, NULL, buffer, &length);
  CHECK_ERROR_CODE("SCardTransmit", rv, SCARD_S_SUCCESS);

  if (tc->check_answer) {
    if ((length!=tc->erxlen) || (memcmp(tc->erx, buffer, length) != 0)) {
//      printf("ERROR: data is not as expected\n");
 //     printf("Received data:\n");
//      dump_buffer(buffer, length);
//      printf("\nExpected data:\n");
//      dump_buffer(tc->erx, tc->erxlen);
      return EXIT_FAILURE;
    }
  } else {
 //     printf("\nReceived data:\n");
//      dump_buffer(buffer, length);
  }
//  printf("OK\n");
  return EXIT_SUCCESS;
}


int cardInit()
{
	char buffer[1024] = "Foobar";
	DWORD cbRecvLength = sizeof(buffer);
	unsigned long active_protocol = 0;

	unsigned long dwReaders;
	unsigned char pbAtr[MAX_ATR_SIZE];
	int i;

//	printf("Estableciendo contexto...\n");
	rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &context);
	if (rv!=EXIT_SUCCESS)
		return rv;

	rv= SCardListReaders (context,NULL,NULL,&dwReaders);
	if (rv!=EXIT_SUCCESS)
		return rv;

	mszReaders = malloc(sizeof (char)*dwReaders);
	rv=SCardListReaders (context, NULL, mszReaders,&dwReaders);

	if (rv!=EXIT_SUCCESS)
		return rv;

//	printf ("Lector = '%s'\n",mszReaders);

	return EXIT_SUCCESS;
}

void cardConnect ()
{
	rv = SCardConnect(context, mszReaders, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &card, &pref);
	/* adquire mutex because we want commands to be executed together */
//	printf("Adquiriendo mutex...\n");
//	rv = SCardBeginTransaction(card);

}

void cardDisconnect()
{
/*---------- FIN --------------*/
	rv = SCardDisconnect (card,SCARD_LEAVE_CARD);
//	rv = SCardEndTransaction(card, SCARD_UNPOWER_CARD);
//	printf("Releasing mutex...\n");
}

void cardEnd()
{
//	printf("Releasing context...\n");
	rv = SCardReleaseContext(context);
	free(mszReaders);
}

long waitForCard (int state)
{
	SCARD_READERSTATE_A rgReaderStates[1];	

	rgReaderStates[0].szReader = mszReaders;
	rgReaderStates[0].dwCurrentState = state;
	rgReaderStates[0].dwEventState=state;

	rv = SCardGetStatusChange(context, INFINITE, rgReaderStates, 1);
	return rgReaderStates[0].dwEventState;
}



int doCard (int *uid)
{
    unsigned char ReadBuffer[500];
	test_command tc[] = 
	{ 
			{"Select MF", (unsigned char *) "\x00\xa4\x00\x00\x02\x3f\x00", 7, 1, (unsigned char *) "\x90\x00", 2},
			{"Read all data in card", (unsigned char *) "\x00\xb0\x00\x00\x00", 5, 0, NULL, 0}, 
			{"Read first 4 bytes (atr)", (unsigned char *) "\x00\xb0\x00\x00\x04",  5, 1, (unsigned char *) "\xa2\x13\x10\x91\x90\x00", 6},
			{"Que posicion toca leer ", (unsigned char *) "\x00\xb0\x00\x25\x01",  5, 0, (unsigned char *) "\xa2\x13\x10\x91\x90\x00", 6},
			{"Leemos UID 1 ", (unsigned char *) "\x00\xb0\x00\x21\x02",  5, 0, (unsigned char *) "\xa2\x13\x10\x91\x90\x00", 6},
			{"Leemos UID 2 ", (unsigned char *) "\x00\xb0\x00\x23\x02",  5, 0, (unsigned char *) "\xa2\x13\x10\x91\x90\x00", 6},
	};
	test_command rw;


//	printf ("Tarjeta insertada ...\n");

//	printf("Testing SCardConnect             : ");

	cardConnect();
//	printf ("Tx tc[0]\n");
//	if(transmit(card,ReadBuffer,500,&tc[0]) != EXIT_SUCCESS)
//	{
//		cardDisconnect();
//		return (EXIT_FAILURE);
//	}

//	if(transmit(card,ReadBuffer,500,&tc[2]) != EXIT_SUCCESS)
//	{
//		cardDisconnect();
//	    return (EXIT_FAILURE); 
//	}
//	printf ("Tx tc[2]\n");

	//Que posición toca leer?
/*	if(transmit(card,ReadBuffer,500,&tc[3]) != EXIT_SUCCESS)
	{
		cardDisconnect();
	    return (EXIT_FAILURE); 
	}*/
	//Leemos UID?
//	if(transmit(card,ReadBuffer,500,&tc[3+ReadBuffer[0]]) != EXIT_SUCCESS)

	if(transmit(card,ReadBuffer,6,&tc[4]) != EXIT_SUCCESS)
	{
		cardDisconnect();
	    return (EXIT_FAILURE); 
	}

	cardDisconnect();
	*uid = MAKEWORD (ReadBuffer[0],ReadBuffer[1]);
	return EXIT_SUCCESS;
}


