CC=gcc
INCLUDES = -I/usr/include/mysql
LIBS = -lpcsclite -lldap -L/usr/lib/mysql -lmysqlclient -lz
all: time


ldap_client.o: ldap_client.c 
	$(CC) -c  ldap_client.c

card.o: card.c card.h
	$(CC) -c card.c

timectrl.o: timectrl.c
	$(CC) -c $(INCLUDES) timectrl.c


time: timectrl.o card.o ldap_client.o 
	$(CC) -o time timectrl.o ldap_client.o card.o $(LIBS)

clean:
	rm -f *.o time 


