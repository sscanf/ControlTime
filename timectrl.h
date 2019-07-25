#ifndef __timectrl__
#define __timectrl__

#define sleep(m) usleep (m*1000)
#define ClrScr() putchar(27);putchar('+');fflush (stdout);sleep (100)
#define Home() putchar(27);putchar('}');fflush (stdout)
#define MYSQL_QUERY(conn,query) if (mysql_query (conn, query)){printf(mysql_error(conn));fflush(stdout);while(1);}

#endif

