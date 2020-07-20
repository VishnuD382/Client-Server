all: ; $(error Must choose client or server!)

client: client.c 
	gcc -o client.out client.c

server: server.c 
	gcc -o server.out server.c -pthread -lm
	
dumbFunc.o: dumbFunc.c
	gcc -c dumbFunc.c
	
LL.o: LL.c
	gcc -c LL.c	

clean:
	rm client; rm server; rm *.o
