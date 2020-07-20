#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //socket
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h> //compile with lpthread

#include "dumbFunc.h"

pthread_mutex_t lock_open;
////////////////////////////////////////////

int helpPrint(int sock)
{
	char *client_message = "\nCommands are:\nquit\ncreate\ndelete\nopen\nclose\nnext\nput\n ";

	write(sock, client_message, strlen(client_message)) - 1;

	return 0;
}

void charReplace(char name[])
{
	int i = 0;
	while (name[i] != '\n')
	{
		i++;
	}
	name[i] = '\0';
}

box_t *createBox(int sock, box_t *firstBox)
{
	char boxName[26];
	box_t *newBox = NULL;

	char *client_message = "What is the name of the box you want to create?\n";
	char *boxCreated = "OK!";
	char *boxCreatedFail = "ER: EXIST";
	char *wrongFormat = "ER:WHAT?";

	write(sock, client_message, strlen(client_message)) - 1;
	recv(sock, boxName, sizeof(boxName), 0);
	printf("boxname %s\n", boxName);

	if (isdigit(boxName[0]) != 0)
	{
		printf("Incorrect Format!\n");
		write(sock, wrongFormat, strlen(wrongFormat)) - 1;
		return newBox;
	}

	while (firstBox != NULL)
	{
		if (strcmp(firstBox->name, boxName) == 0)
		{
			write(sock, boxCreatedFail, strlen(boxCreatedFail));
			return newBox;
		}
		firstBox = firstBox->next;
	}

	newBox = malloc(sizeof(box_t));
	strcpy(newBox->name, boxName);
	newBox->next = NULL;
	newBox->firstMessage = NULL;
	newBox->openStatus = 1;
	pthread_mutex_init(&newBox->lock, NULL);

	printf("%s\n", newBox->name);
	write(sock, boxCreated, strlen(boxCreated));

	return newBox;
}

int deleteBox(box_t *box)
//mkae a remove box
{
	messages_t *message = box->firstMessage;
	messages_t *temp = NULL;
	if (message == NULL)
	{
		free(box);
		return 1;
	}
	while (message != NULL)
	{
		temp = message;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		free(temp);
	}
	free(box);
	return 1;
}

box_t *openBox(int sock, box_t *firstBox)
{
	char *client_message = "What box do you want to open?";
	char boxName[26];
	char *error = "ER: CNTOPEN"; //error message in if statemetn
	char *open = "open";
	write(sock, client_message, strlen(client_message)) - 1;
	recv(sock, boxName, sizeof(boxName), 0);

	box_t *ptr = NULL;

	while (firstBox)
	{
		printf("boxes: %s\n", firstBox->name);

		int check = strcmp(firstBox->name, boxName);
		printf("boxes %s, check %d\n", boxName, check);

		if (strcmp(firstBox->name, boxName) == 0)
		{
			if (firstBox->openStatus == 0)
			{
				write(sock, error, strlen(error));
			}
			firstBox->openStatus = 0;
			pthread_mutex_lock(&firstBox->lock); //open mutex
			write(sock, open, strlen(open));
			return firstBox;
		}
		firstBox = firstBox->next;
	}

	char *dnexist = "Box does not exist!";
	write(sock, dnexist, strlen(dnexist));
	return ptr;
}

int closeBox(int sock, box_t *curr)
{
	char *client_message = "What box do you want to close?";
	char boxName[26];
	char *fail = "ER:NOOPN";
	char *success = "OK!";
	write(sock, client_message, strlen(client_message)) - 1;
	recv(sock, boxName, 26, 0);

	if (strcmp(curr->name, boxName) == 0)
	{
		pthread_mutex_unlock(&curr->lock); //close mutex
		curr->openStatus = 1;
		write(sock, success, strlen(success));

		return 1;
	}
	write(sock, fail, strlen(fail));
	return 0;
}

int nextMessage(int sock, box_t *curr)
{

	char *client_message = "Next Message\n";
	char *error = "ER: "; //finsih

	write(sock, client_message, strlen(client_message));

	if (curr->firstMessage == NULL)
	{
		write(sock, error, strlen(error));
		return 0;
	}

	messages_t *message = curr->firstMessage;
	write(sock, message->inMessage, strlen(message->inMessage));

	curr->firstMessage = curr->firstMessage->next;

	free(message);

	return 1;
}

int putMessage(int sock, box_t *curr)
{
	messages_t *currMessage = curr->firstMessage;

	char message[2000];

	char *client_message = "Enter your message: \n";

	write(sock, client_message, strlen(client_message)); //check -1
	recv(sock, message, 2000, 0);

	if (currMessage == NULL)
	{
		currMessage = malloc(sizeof(messages_t));
		strcpy(currMessage->inMessage, message);
		currMessage->next = NULL;
		return 1;
	}

	while (currMessage->next != NULL)
	{
		currMessage = currMessage->next;
	}
	currMessage->next = malloc(sizeof(messages_t));
	strcpy(currMessage->next->inMessage, message);
	currMessage->next->next = NULL;

	return 0;
}