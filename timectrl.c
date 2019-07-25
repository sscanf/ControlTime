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
#include <pcsclite.h>
#include <my_global.h>
#include <mysql.h>
#include <time.h>
#include <curses.h>
#include "timectrl.h"
#include <pthread.h>

static char *opt_host_name	= NULL;		/* server host (default=localhost) */
static char *opt_user_name	= NULL;		/* username (default=login name) */
static char *opt_password	= NULL;		/* password (default=none) */
static unsigned int opt_port_num = 0;	/* port number (use built-in value) */
static char *opt_socket_name= NULL;		/* socket name (use built-in value) */
static char *opt_db_name	= NULL;		/*database name (default=none)*/
static unsigned int opt_flags=0;		/*connection flags (none)*/
static MYSQL *conn;
long rv;

void MsgAviso (char *str, char beep)
{
	char buffer[10];
	printf ("%s",str);
	fflush(stdout);

	switch (beep)
	{
		case 1:
			putchar (0x7);	fflush(stdout);
			sleep (200);	
			putchar (0x8);	fflush(stdout);
			sleep (50);
			putchar (0x7);	fflush(stdout);
			sleep (200);
			putchar (0x8);	fflush(stdout);
			sleep (50);
			putchar (0x7);	fflush(stdout);
			sleep (200);
			putchar (0x8);	fflush(stdout);
		break;
	
		case 2:
			putchar (0x7);	fflush(stdout);
			sleep (200);	
			putchar (0x8);	fflush(stdout);
		break;

	}
	
	sleep (100);
}


void print_error (MYSQL *conn, char *message)
{
	fprintf (stderr,"%s\n\r", message);
	
	if (conn!=NULL)
	{
		fprintf (stderr, "Error %u (%s)\n\r",
			mysql_errno (conn), mysql_error (conn));
	}
}

void process_result_set (MYSQL *conn, MYSQL_RES *res_set)
{
	MYSQL_ROW	row;
	unsigned int	i;

	while ((row=mysql_fetch_row (res_set))!=NULL)
	{
		for (i=0;i<mysql_num_fields (res_set);i++)
		{
			if (i>0)
				fputc ('\t',stdout);

			printf ("%s",row[i]!=NULL?row[i]:"NULL");
		}
		fputc ('\n\r',stdout);
	}

	if (mysql_errno (conn) !=0)
		print_error (conn,"mysql_fetch_row() failed");
	else
		printf ("%lu rows returned\n\r",(unsigned long)mysql_num_rows (res_set));
}


int sqlDisconnect()
{
	/*disconnect from server */
	mysql_close (conn);
}

void *functionThread (void)
{
	while (1)
		rv = waitForCard(SCARD_STATE_EMPTY);
}

int main()
{
	char banner[]={"Introduzca su tarjeta con los contactos hacia arriba. "};
	char buff[33];
	char head=0;
	char tail=0;
	int n,tmp=0;

	char str[1024];
	char strHorarioActual[5];
	char strHoraApertura [80];
	char strID[5];
	char **vars;
	int uid,i;
	MYSQL_ROW row;
	MYSQL_RES *result_set;
	struct tm *ptrTm;
	time_t the_time;
	struct tm tHoraEntrada;
	struct tm tHoraSalida;
   pthread_t thread;
   int error;

	ClrScr();
	
	conn = mysql_init (conn);
	if (conn==NULL)
	{
		fprintf (stderr, "mysql_init() failed (probably out of memory)\n\r");
		return (-1);
	}

	/*Connect to server */
	if (mysql_real_connect (conn, opt_host_name,"oscar","optr2c", "ebsl_dbo",
			opt_port_num,opt_socket_name, opt_flags)==NULL)
	{
		fprintf (stderr,"mysql_real_connect() failed:\n\rError =%u (%s)\n\r",
			mysql_errno (conn),mysql_error (conn));
		mysql_close (conn);
		return (-1);
	}

	MYSQL_QUERY (conn, "SELECT HorarioActual FROM ConfigHorario where id = (select max(id) from ConfigHorario)");
	
	result_set = mysql_store_result (conn);
	if (mysql_num_rows (result_set)==0)
	{
			fprintf (stderr, "Error, no puedo leer el horario actual\n\r");
			exit(1);
	}

	if (result_set == NULL)
	{
		 printf(mysql_error(conn));
		exit(1);
	}
	row=mysql_fetch_row (result_set);
	strcpy (strHorarioActual,row[0]);

	mysql_free_result (result_set);

	if (!strcmp (strHorarioActual,"P"))
		strcpy (strHoraApertura,"1900/1/1 8:00:00");

	if (!strcmp (strHorarioActual,"I"))
		strcpy (strHoraApertura,"1900/1/1 7:00:00");

	if (cardInit()!=EXIT_SUCCESS)
	{
		fprintf (stderr,"Error en la inicializacion del lector \n\r");
		exit( 1);
	}

    error = pthread_create (&thread,NULL,functionThread, NULL);
	for (; ; )
	{
		ClrScr();
		head=0;
		printf ("\x1b'0");		//Terminal Cursor OFF
		fflush (stdout);
		do
		{
		   time (&the_time);
			ptrTm = localtime (&the_time);
			Home(); 
			printf ("   Electronica Barcelona,S.l.\n\r");
			printf ("        Control horario\n\r\n\r");
			printf ("%02d/%02d/%04d              %02d:%02d.%02d\n\r\n\r",
				ptrTm->tm_mday,ptrTm->tm_mon+1,ptrTm->tm_year+1900,
				ptrTm->tm_hour,ptrTm->tm_min,ptrTm->tm_sec);
			fflush(stdout);
			(!strcmp (strHorarioActual,"P")) ? 
			printf ("        Horario partido\n\r\n\r"): 
			printf ("       Horario intensivo\n\r");
			if (tmp==8)
			{
				printf ("       -----------------  \n\r");
				printf ("      |   ------------  | \n\r");
				printf ("       ---|    |||   |--  \n\r");
				printf ("          |          |    \n\r");
				printf ("          |          |    \n\r");
				printf ("          |          |    \n\r");
			}
			else if (tmp==16)
			{
				tmp=0;
				printf ("       -----------------  \n\r");
				printf ("      |   ============  | \n\r");
				printf ("       -----------------  \n\r");
				printf ("           ----------     \n\r");
				printf ("          |          |    \n\r");
				printf ("          |    |||   |    \n\r");
			}
			else
				printf ("\n\r\n\r\n\r\n\r\n\r\n\r");

			tmp++;
	
			tail=head;
			for (n=0;n<32;n++)
			{
				buff[n]=banner[head++];
				if (head>strlen (banner)-1)
				 head=0;
			}
			
			head=tail+1;
			if (head>strlen (banner)) head=0;
			
			buff[33]=0;
		    printf ("\n\r\n\r%s",buff);
			fflush(stdout);
						
			sleep (250);
		}while(!(rv&SCARD_STATE_PRESENT));
		printf ("\x1b'1");		//Terminal Cursor ON
		ClrScr();
		printf ("   Electronica Barcelona,S.l.\r");
		printf ("        Control horario\n\r\n\r\n\r");
		printf (" LEYENDO TARJETA ...",0);

		if (doCard(&uid)==EXIT_SUCCESS)
		{
			ClrScr();
			vars = LDAP_search (uid);
		   
		  if (vars == -1)
					 exit (0);

			if (strlen (vars[0])==0)
			{
				
				sprintf (str,"   Tarjeta no identificada\n\r");
				ClrScr();
				printf ("   Electronica Barcelona,S.l.\r");
				printf ("        Control horario\n\r");

				printf ("              ERROR       \r");
				printf ("       --\\----------/--  \r");
				printf ("      |   =\\======/=   | \r");
				printf ("       ------\\--/------  \r");
				printf ("           ----\\----    \r");
				printf ("          |  /   \\  |    \r");
				printf ("          |/       \\     \r\r");
				MsgAviso (str,1);
			}
			else
			{
	         time (&the_time);
   	      ptrTm = localtime (&the_time);

				ClrScr();
				if (ptrTm->tm_hour<12)
				 	printf ("          Buenos dias\n\r");
				else
					printf ("          Buenas tardes\n\r");

				sprintf (str,"%s %s (%d)\n\r",vars[1],vars[0],uid);
				MsgAviso (str,0);

				//Miramos si toca Entrada o Salida y lo apuntamos

				sprintf (str,"SELECT id,HoraEntrada,HoraSalida FROM ControlHorario where "
							"operarioid='%d' and "
							"day(fecha)='%d' and "
							"month(fecha)='%d' and "
							"year(fecha)='%d'",
							uid,
							ptrTm->tm_mday,
							ptrTm->tm_mon+1,
							ptrTm->tm_year+1900);

	//			printf ("%s\n\r",str);
				MYSQL_QUERY (conn,str);
				result_set = mysql_store_result (conn);

				printf ("Registro de hoy\r");
				printf ("-------------------------------\r");
				if (mysql_num_rows (result_set))
				{
					while (row=mysql_fetch_row (result_set))
					{
						if (row[1])
						{
							strptime(row[1], "%Y-%m-%d %H:%M:%S", &tHoraEntrada);
							printf ("Entrada: %02d:%02d  ",tHoraEntrada.tm_hour,tHoraEntrada.tm_min);							
						}

						if (row[2])
						{
							strptime(row[2], "%Y-%m-%d %H:%M:%S", &tHoraSalida);
							printf ("Salida: %02d:%02d\r",tHoraSalida.tm_hour,tHoraSalida.tm_min);
						}
						else
							printf ("\r");
					}
					printf ("------------------------------\r");
					fflush (stdout);


					sprintf (str,"SELECT id,HoraEntrada,HoraSalida FROM ControlHorario where "
								"operarioid='%d' "
								"and (fecha=(select max(fecha) from ControlHorario where "
								"day(fecha)='%d' "
								"and month(fecha)='%d' "
								"and year(fecha)='%d' and operarioid='%d'))",
								uid,
								ptrTm->tm_mday,
								ptrTm->tm_mon+1,
								ptrTm->tm_year+1900,
								uid);
					MYSQL_QUERY (conn,str);
					result_set = mysql_store_result (conn);
					row=mysql_fetch_row (result_set);
					strcpy (strID,row[0]);

//					strptime(row[1], "%Y-%m-%d %H:%M:%S", &tHoraEntrada);
//					strptime(row[2], "%Y-%m-%d %H:%M:%S", &tHoraSalida);

					if (row[1]==0) //Es Entrada
					{
						sprintf (str,"update ControlHorario set HoraEntrada = now() where id=%s",strID);
						MYSQL_QUERY (conn,str);
						strftime(str, 128, "\r ENTRADA: %H:%M:%S ", ptrTm);
						MsgAviso (str,2);
						fflush (stdout);
					}

					if (row[1]>0 && row[2]==0) //Es salida
					{
						sprintf (str,"update ControlHorario set HoraSalida= now() where id=%s",strID);
//						printf ("%s\n\r",str);
						MYSQL_QUERY (conn,str);
						strftime(str, 128,"\r SALIDA: %H:%M:%S", ptrTm);
						MsgAviso (str,2);
						fflush(stdout);
					}

					if (row[1]>0 && row[2]>0)	//Entrada en otra l�ea
					{
//						printf ("%c\n\r\n\r",strHorarioActual);
						sprintf (str,"insert into ControlHorario (OperarioID,Fecha,HoraEntrada,TipoHorario,HoraApertura) "
							"values (%d,now(),now(),'%s','%s')",uid,strHorarioActual,strHoraApertura);
//						printf ("%s\n\r",str);
						MYSQL_QUERY (conn,str);
						strftime(str, 128, "\r ENTRADA: %H:%M:%S", ptrTm);
						MsgAviso (str,2);
						fflush(stdout);
					}
				}
				else
				{
					sprintf (str,"insert into ControlHorario (OperarioID,Fecha,HoraEntrada,TipoHorario,HoraApertura) "
						"values (%d,now(),now(),'%s','%s')",uid,strHorarioActual,strHoraApertura);
			//		printf ("%s\n\r",str);
					MYSQL_QUERY (conn,str);
					strftime(str, 128, "ENTRADA: %H:%M:%S", ptrTm);
					MsgAviso (str,2);
					fflush (stdout);
				}
			}
			free (vars[0]);
			free (vars[1]);
			free (vars);

		}
		else
		{
			ClrScr();
			MsgAviso ("Tarjeta mal insertada\n\rInsertela correctamente\n\r",1);	
		}

		do
		{
			printf ("\r\rRetire su tarjeta");
			fflush (stdout);	
			rv = waitForCard(SCARD_STATE_PRESENT);
			ClrScr();
		}while(rv&SCARD_STATE_PRESENT);
	}
}
