/* $Id: config.h,v 1.6 2005/12/20 16:46:14 jmt Exp $ */
/*
copyright 1997, 2001
the regents of the university of michigan
all rights reserved

permission is granted to use, copy, create derivative works 
and redistribute this software and such derivative works 
for any purpose, so long as the name of the university of 
michigan is not used in any advertising or publicity 
pertaining to the use or distribution of this software 
without specific, written prior authorization.  if the 
above copyright notice or any other identification of the 
university of michigan is included in any copy of any 
portion of this software, then the disclaimer below must 
also be included.

this software is provided as is, without representation 
from the university of michigan as to its fitness for any 
purpose, and without warranty by the university of 
michigan of any kind, either express or implied, including 
without limitation the implied warranties of 
merchantability and fitness for a particular purpose. the 
regents of the university of michigan shall not be liable 
for any damages, including special, indirect, incidental, or 
consequential damages, with respect to any claim arising 
out of or in connection with the use of the software, even 
if it has been or is hereafter advised of the possibility of 
such damages.
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__


/* buffer size for internal usage */
#define LTC3X_INTERNAL_BUFFER_SIZE 512

/* timeout to read echo characters (in ms) */
#define LTC3X_ECHO_TIMEOUT 200

/* time between dtr and command (in ms) */
#define LTC3X_TIME_BETWEEN_DTR_AND_COMMAND 1

/* time between commands (in ms) */
#define LTC3X_TIME_BETWEEN_COMMANDS 20

/* guardtime before sync commands (in ms) */
#define LTC3X_SYNC_COMMAND_GUARDTIME 5

/* timeout for ACK in synchronous command mode (in ms) */
#define LTC3X_SYNC_ACK_TIMEOUT 100

/* guartime between bytes in sync command mode (in ms) */
#define LTC3X_SYNC_CHAR_GUARDTIME 1

/* timeout for response in sync commands (in ms)... */
#define LTC3X_SYNC_RESPONSE_TIMEOUT 5000


/* used in GCdebug.h output routines */
#define READER_NAME "ltc3x"

/* debug message level */
 #define DEBUG_LEVEL_CRITICAL
 #define DEBUG_LEVEL_INFO 
 #define DEBUG_LEVEL_PERIODIC 
 #define DEBUG_LEVEL_COMM 

#endif /* __CONFIG_H__ */
