all :  keys client server
keys:
	gcc -Wall -c keys.c -pthread -lrt
	ar -rv libkeys.a keys.o
client: keys
	gcc -Wall -o client client.c libkeys.a -lrt
server:
	gcc -Wall -c TripletLinkedList.c -lrt
	gcc -Wall -o server server.c TripletLinkedList.o -pthread -lrt

clean :
	rm -f client
	rm -f server
	rm -f libkeys.a
	rm -f *.o
