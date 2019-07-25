#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ldap.h>

LDAP *ld;
int auth_method = LDAP_AUTH_SIMPLE;
int desired_version = LDAP_VERSION2;
char *ldap_host = "localhost";
char *root_dn = "cn=Manager, dc=arvag.ebsl,dc=com";
char *root_pw = "prueba";
char *base = "ou=People,dc=arvag.ebsl,dc=com";
char *errstring;
char *dn=NULL;
char *attr;
char **vals;
int i;
int result;

BerElement *ber;
LDAPMessage *msg;
LDAPMessage *entry;

char **LDAP_search (int id)
{
	char **results;
	char filter[80];

//	printf("Conectando con LDAP ...\n");

	results= (char **)malloc (2*sizeof (char*));
	results[0] = (char *)malloc (40*sizeof (char*));
	results[1] = (char *)malloc (40*sizeof (char*));

	memset (results[0],0,40);
	memset (results[1],0,40);
	sprintf (filter,"(uidNumber=%d)",id);

//	printf ("Filtro = %s \n",filter);

	if ((ld = ldap_init (ldap_host,NULL))==NULL)
	{
		printf ("Inicializacion LDAP fallida\n");
	    return -1;
	}

	if (ldap_set_option (ld,LDAP_OPT_PROTOCOL_VERSION,&desired_version)!=LDAP_SUCCESS)
	{
		ldap_perror (ld,"ldap_set_option");
		return -1;
	}

	if (ldap_bind_s (ld, root_dn, root_pw, auth_method)!=LDAP_SUCCESS)
	{
		ldap_perror (ld,"ldap_bind");
		return -1;
	}

	if (ldap_search_s (ld, base, LDAP_SCOPE_SUBTREE,filter,NULL,0,&msg) != LDAP_SUCCESS)
	{
		ldap_perror (ld, "ldap_search_s");
		return -1;
	}

//	printf ("El numero de entradas devueltas es de :%d\n\n",ldap_count_entries(ld,msg));


	for (entry=ldap_first_entry (ld,msg);entry!=NULL;entry=ldap_next_entry(ld,entry))
	{
		if ((dn=ldap_get_dn (ld,entry))!=NULL)
		{
//			printf ("Returned dn: %s\n",dn);
			ldap_memfree(dn);
		}

		for (attr = ldap_first_attribute(ld,entry,&ber);attr!=NULL;attr=ldap_next_attribute (ld,entry,ber))
		{
			if ((vals=ldap_get_values(ld,entry,attr))!=NULL)
			{
				for (i=0; vals[i]!=NULL; i++ )
				{
					if (strcmp (attr,"sn")==0)
					{
						strcpy (results[0],vals[i]);
//						printf ("%s\n",vals[i]);
					}

					if (strcmp (attr,"cn")==0 )
					{
						strcpy (results[1],vals[i]);
//						printf ("%s\n",vals[i]);
					}
				}
				ldap_value_free(vals);
			}
			ldap_memfree (attr);
		}
	}
	if (ber!=NULL)
	{
		ber_free (ber,0);
	}
//	printf ("\n");

	ldap_msgfree (msg);

	result = ldap_unbind_s(ld);
	if (result!=0)
	{
		fprintf (stderr, ",ldap_unbind_s: %s",ldap_err2string (result));
		return -1;
	}
	return results;
}

