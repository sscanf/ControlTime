#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include <timectrl.h>


static char *opt_host_name	= NULL;		/* server host (default=localhost) */
static char *opt_user_name	= NULL;		/* username (default=login name) */
static char *opt_password	= NULL;		/* password (default=none) */
static unsigned int opt_port_num = 0;	/* port number (use built-in value) */
static char *opt_socket_name= NULL;		/* socket name (use built-in value) */
static char *opt_db_name	= NULL;		/*database name (default=none)*/
static unsigned int opt_flags=0;		/*connection flags (none)*/
static MYSQL *conn;


void print_error (MYSQL *conn, char *message)
{
	fprintf (stderr,"%s\n", message);
	
	if (conn!=NULL)
	{
	   ClrScr();
		sleep (500);
		fprintf (stderr, "Error %u (%s)\n",
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
		fputc ('\n',stdout);
	}

	if (mysql_errno (conn) !=0)
		print_error (conn,"mysql_fetch_row() failed");
	else
		printf ("%lu rows returned\n",(unsigned long)mysql_num_rows (res_set));
}

int sqlConnect()
{
	/* Initialize connection handler */
	conn = mysql_init (conn);
	if (conn==NULL)
	{
		fprintf (stderr, "mysql_init() failed (probably out of memory)\n");
		return (-1);
	}

	/*Connect to server */
	if (mysql_real_connect (conn, opt_host_name,opt_user_name,"!beer4u", "ebsl_dbo",
			opt_port_num,opt_socket_name, opt_flags)==NULL)
	{
		fprintf (stderr,"mysql_real_connect() failed:\nError =%u (%s)\n",
			mysql_errno (conn),mysql_error (conn));
		mysql_close (conn);
		return (-1);
	}
	return EXIT_SUCCESS;
}

int sqlQuery (char *query, MYSQL_RES *result_set)
{
	int i;

	if (mysql_query (conn, query)!=0)
	{
		print_error (conn,"mysql_query() failed");
		return -1;
	}
	else
	{
		result_set = mysql_store_result (conn);	/*Generate result set */
		if (result_set == NULL)
		{
			print_error (conn,"my_sql_store_result() failed");
			return -1;
		}
	}
	return 0;
}

int sqlDisconnect()
{
	/*disconnect from server */
	mysql_close (conn);
}
