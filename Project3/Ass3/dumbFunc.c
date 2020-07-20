#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //socket
#include <unistd.h>
#include <time.h>
#include <pthread.h> //compile with lpthread

#include "dumbFunc.h"

int helpPrint(int sock)
{

	char *client_message = "\nCommands are:\nquit\ncreate\ndelete\nopen\nclose\nnext\nput\n ";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;

}

int createMessage(int sock)
{

	char *client_message = "Create\n";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;

}
int deleteMessage(int sock)
{

	char *client_message = "Delete\n";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;

}
int openBox(int sock)
{

	char *client_message = "OpenBox\n";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;

}
int closeBox(int sock)
{

	char *client_message = "Close Box\n";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;

}
int nextMessage(int sock)
{

	char *client_message = "Next Message\n";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;

}
int putMessage(int sock)
{

	char *client_message = "Put Message\n";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;


}