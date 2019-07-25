/* $Id: test_c3p2k.c,v 1.2 2006/02/02 11:11:52 jmt Exp $ */

/*
  Test of C3P2K card implementation in LTC3X driver

  Josep Mones i Teixidor
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winscard.h>
#include <pcsclite.h>

#define CHECK_ERROR_CODE(function,rv,expected) \
{                     \
  if ((LONG)(rv)!=(LONG)(expected)) { \
    printf("ERROR: \"%s\" returned 0x%lX and we expected 0x%lX\n", function, (LONG)(rv), (LONG)(expected)); \
    exit(EXIT_FAILURE); \
  } \
}


#define DO_TRANSMIT(card,tc) \
{                            \
  if(transmit(card,tc) != EXIT_SUCCESS) \
    exit(EXIT_FAILURE);      \
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

int transmit(SCARDHANDLE card, const test_command *tc)
{
  LONG rv;
  unsigned char buffer[500];
  DWORD length = sizeof(buffer);

  printf("%s... ", tc->comment);
  fflush(stdout);
  rv = SCardTransmit(card, SCARD_PCI_T0, tc->tx, tc->txlen, NULL, buffer, &length);
  CHECK_ERROR_CODE("SCardTransmit", rv, SCARD_S_SUCCESS);

  if (tc->check_answer) {
    if ((length!=tc->erxlen) || (memcmp(tc->erx, buffer, length) != 0)) {
      printf("ERROR: data is not as expected\n");
      printf("Received data:\n");
      dump_buffer(buffer, length);
      printf("\nExpected data:\n");
      dump_buffer(tc->erx, tc->erxlen);
      return EXIT_FAILURE;
    }
  } else {
      printf("\nReceived data:\n");
      dump_buffer(buffer, length);
  }
  printf("OK\n");
  return EXIT_SUCCESS;
}


int main()
{
  int len, off;
  unsigned long active_protocol = 0;
  char *readers = NULL;
  unsigned long readers_length = 0;
  SCARDCONTEXT context;
  SCARDHANDLE card;
  DWORD pref;
  LONG rv;
  char comment[100];
  unsigned char rbuffer[300];
  unsigned char wbuffer[300];
  test_command tc[] = { {"Select MF", (unsigned char *) "\x00\xa4\x00\x00\x02\x3f\x00", 7, 1, (unsigned char *) "\x90\x00", 2},
 			{"Read all data in card", (unsigned char *) "\x00\xb0\x00\x00\x00", 5, 0, NULL, 0}, 
			{"Read first 4 bytes (atr)", (unsigned char *) "\x00\xb0\x00\x00\x04",  5, 1, (unsigned char *) "\xa2\x13\x10\x91\x90\x00", 6},
			{"Test bad CLA", (unsigned char *) "\x01\xb0\x00\x00\x04",  5, 1, (unsigned char *) "\x6e\x00", 2},
			{"Test bad INS", (unsigned char *) "\x00\xb3\x00\x00\x04",  5, 1, (unsigned char *) "\x6d\x00", 2},
			{"Check number of attempts for pin", (unsigned char *) "\x00\x20\x00\x00\x00", 5, 1, (unsigned char *) "\x63\xc3", 2},
			{"Verify a pin (ffffff)", (unsigned char *) "\x00\x20\x00\x00\x03\xff\xff\xff", 8, 1, (unsigned char *) "\x90\x00", 2},
			{"Reset card", NULL, 0, 0, NULL, 0},
			{"Change pin (ffffff->010203)", (unsigned char *) "\x00\x24\x00\x00\x06\xff\xff\xff\x01\x02\x03", 0x0b, 1, (unsigned char *) "\x90\x00", 2},
			{"Reset card", NULL, 0, 0, NULL, 0},
		     	{"Verify a pin (010203)", (unsigned char *) "\x00\x20\x00\x00\x03\x01\x02\x03", 8, 1, (unsigned char *) "\x90\x00", 2},
			{"Change pin (010203->ffffff)", (unsigned char *) "\x00\x24\x00\x00\x06\x01\x02\x03\xff\xff\xff", 0x0b, 1, (unsigned char *) "\x90\x00", 2},
			{"Reset card", NULL, 0, 0, NULL, 0},
		        {"Verify a pin (ffffff)", (unsigned char *) "\x00\x20\x00\x00\x03\xff\xff\xff", 8, 1, (unsigned char *) "\x90\x00", 2}
  };
  test_command rw;

  const int num_commands = 0xe;
  int ii;

  printf("Tests for C3P2K support in LTC3x serial reader.\n");
  printf("WARNING: can block your card if pin is not FF FF FF\n");
  
  printf("Establishing context...\n");
  rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &context);
  CHECK_ERROR_CODE("SCardExtablishContext", rv, SCARD_S_SUCCESS);

  printf("Finding first reader...\n");
  rv = SCardListReaders(context, NULL, readers, &readers_length);
  CHECK_ERROR_CODE("SCardListReaders", rv, SCARD_S_SUCCESS);

  readers = malloc(readers_length);
  if (readers == NULL) {
    printf("ERROR: allocating memory\n");
    return EXIT_FAILURE;
  }
  
  /* retry now with a buffer */
  rv = SCardListReaders(context, NULL, readers, &readers_length);
  CHECK_ERROR_CODE("SCardListReaders", rv, SCARD_S_SUCCESS);
  
  /* Now readers has a list of readers. We use the first one.
     If no reader is installed readers will be \0 and ScardConnect will fail.
  */
  printf("Connecting to reader \"%s\"...\n", readers);
  rv = SCardConnect(context, readers, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, &card, &pref);
  CHECK_ERROR_CODE("SCardConnect", rv, SCARD_S_SUCCESS);

  /* adquire mutex because we want commands to be executed together */
  printf("Adquiring mutex...\n");
  rv = SCardBeginTransaction(card);
  CHECK_ERROR_CODE("SCardBeginTransaction", rv, SCARD_S_SUCCESS);
  
  
  for (ii=0; ii<num_commands; ii++) {
    if (strcmp(tc[ii].comment, "Reset card") == 0) {
      printf("Reset card...");
      rv = SCardReconnect(card, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0, SCARD_RESET, &active_protocol);
      CHECK_ERROR_CODE("SCardReconnect", rv, SCARD_S_SUCCESS);
      printf(" OK\n");
    } else {
      DO_TRANSMIT(card,&tc[ii]);
    }
  }

  /* initialize command structs */
  rw.comment = comment;
  rw.tx = wbuffer;
  rw.erx = rbuffer;
  rw.check_answer = 1;
  for (off=0x20; off<0x100; off+=57) {
    for (len=(0x100-off); len>0;len-=23) {
      /* we generate write command */
      sprintf(comment, "Writing 0x%02X bytes at 0x%02X", len, off);
      memcpy(wbuffer, "\x00\xd6\x00", 3);
      wbuffer[3] = off;
      wbuffer[4] = len;
      for (ii=0; ii<len; ii++)
	wbuffer[5+ii] = ii;
      rw.erxlen = 2;
      rw.txlen = len+5;
      memcpy(rbuffer, "\x90\x00", 3);
      DO_TRANSMIT(card,&rw);

      /* we check back the answer */
      sprintf(comment, "Reading back 0x%02X bytes at 0x%02X", len, off);
      memcpy(wbuffer, "\x00\xb0\x00", 3);
      wbuffer[3] = off;
      wbuffer[4] = len;
      for (ii=0; ii<len; ii++)
	rbuffer[ii] = ii;
      rbuffer[len] = 0x90;
      rbuffer[len+1] = 0x00;
      rw.txlen = 5;
      rw.erxlen = len+2;
      DO_TRANSMIT(card,&rw);
    }
  }

     

  /* release mutex */
  printf("Releasing mutex...\n");
  rv = SCardEndTransaction(card, SCARD_UNPOWER_CARD);
  CHECK_ERROR_CODE("SCardEndTransaction", rv, SCARD_S_SUCCESS);

  printf("Releasing context...\n");
  rv = SCardReleaseContext(context);
  CHECK_ERROR_CODE("SCardReleaseContext", rv, SCARD_S_SUCCESS);
  

  free(readers);
  printf("Bye!\n");
  return EXIT_SUCCESS;
}
