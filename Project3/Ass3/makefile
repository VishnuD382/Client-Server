all: ; $(error Must choose client or server!)

client: client.c 
	gcc -o client.out client.c

server: server.c dumbFunc.o
	gcc -o server.out server.c dumbFunc.o -pthread -lm
	
dumbFunc.o: dumbFunc.c
	gcc -c dumbFunc.c

clean:
	rm client; rm server; rm *.o
