all: UdpProxy

UdpProxy: main.o server.o usage.o
	gcc -o UdpProxy *.o

server.o: server.c server.h
	gcc -c server.c
	
usage.o: usage.c usage.h
	gcc -c usage.c

main.o: main.c
	gcc -c main.c
	
clean:
	rm *.o UdpProxy