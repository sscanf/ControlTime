#include <hc05c9_r.h>

#define FALSE 0
#define TRUE  1

#define lda 0xD6        /* indexed long */
#define sta 0xD7        /* indexed long */
#define jmp 0xCC        /* ext */
#define rts 0x81
#define nop asm nop

#define FRAME   0xeca1
#define LEN_MSG 96 //Largura máxima de los mensajes del DTE

// Definición de mensajes

#define MSG_ACT   0xff    //Activación
#define MSG_DES   0xfe    //Desactivación
#define MSG_ACK   0xc0    //ACK
#define MSG_NACK  0xc1    //NACK
#define MSG_OFF   0xc1    //Mensaje para desconectar el link
#define MSG_PDTS  0xd0    //Petición de datos
#define MSG_CID   0xfd    //Cambio de indicativo
#define MSG_PID   0xe0    //Programación de indicativo
#define MSG_TXD   0xd4    //Envío de datos
#define MSG_PTT   0x01    //PTT
#define MSG_TEST  0x02    //Test


#define CTS       2  //Clear To Send (RS232)

char SendWordSPI (unsigned long byte);
char SendByteSPI (unsigned char byte);

