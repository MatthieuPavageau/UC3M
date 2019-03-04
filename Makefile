all : client server keys.o
client: client.c keys.o
	gcc -Wall -o client  client.c keys.o -pthread -lrt
server: server.c
	gcc -Wall -o server server.c -pthread -lrt
keys.o  : keys.c	
	gcc -Wall -c keys.c -pthread -lrt
clean :
	rm -f client
	rm -f server
	rm -f libkeys.a
	rm -f *.o
