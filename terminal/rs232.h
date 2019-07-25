#include <hc05c9_r.h>

#define BUFFER_SIZE 127
#define RS485_ENABLE	b_PC1


char RS_kbhit(void);
void RS_Init(unsigned char baud, unsigned char options);
unsigned char RS_GetChar(void);
void RS_PutChar(char ch);
void RS232_isr ();
void RS_Enable(char enable);
void RS_PutString (char *str, int len);
