all :
	gcc -Wall -c keys.c
	ar -rv libkeys.a keys.o
	gcc -Wall -o client client.c libkeys.a 
	gcc -Wall -o server server.c

clean :
	rm -f client
	rm -f server
	rm -f libkeys.a
	rm -f *.o
