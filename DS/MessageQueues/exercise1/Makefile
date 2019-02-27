all :
	gcc -Wall -c keys.c -lrt
	ar -rv libkeys.a keys.o
	gcc -Wall -o client client.c libkeys.a -lrt
	gcc -Wall -o server server.c -pthread -lrt

clean :
	rm -f client
	rm -f server
	rm -f libkeys.a
	rm -f *.o
