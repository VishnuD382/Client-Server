#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dumbFunc.c"

void addBox(box_t *first, int sock)
{
    // Insert new item at the end.
    box_t *curr = first;

    while (curr->next)
    {
        curr = curr->next;
    }
    curr->next = createBox(sock, first);

    curr = first;

    // while (curr)
    // {
    //     printf("boxes: %s\n", curr->name);
    //     curr = curr->next;
    // }
}

box_t *removeBox(box_t *first, int sock)
{
    char boxName[26];
    char *client_message = "What is the name of the box you want to delete?\n";
    char *notFound = "File not found";
    char *error = "No box to delete";

    write(sock, client_message, strlen(client_message));
    recv(sock, boxName, 2000, 0);

    box_t *temp = NULL;

    if (first == NULL)
    {
        write(sock, error, sizeof(error));
        return temp;
    }

    if (strcmp(first->name, boxName) == 0)
    {
        pthread_mutex_lock(&first->lock);
        temp = first;
        first = first->next;
        deleteBox(temp);
        return first;
    }

    box_t *prev = first;
    box_t *curr = first->next;
    while (curr != NULL)
    {
        if (strcmp(curr->name, boxName) == 0)
        {
            if (curr->openStatus == 0)
            {
                write(sock, error, strlen(error));
            }
            curr->openStatus = 0;
            pthread_mutex_lock(&first->lock);
            temp = curr;
            prev->next = curr->next;
            deleteBox(temp);
            return first;
        }
        prev = prev->next;
        curr = curr->next;
    }

    write(sock, notFound, sizeof(notFound));
    return first;
}

void dumpDetails(box_t *currBox)
{
    // For every child.

    while (currBox != NULL)
    {
        printf("%s has:\n", currBox->name);

        // For every toy that child has.

        messages_t *currMessage = currBox->firstMessage;
        if (currMessage == NULL)
        {
            printf("   <<nothing>>\n");
        }
        else
        {
            while (currMessage != NULL)
            {
                printf("   %s\n", currMessage->inMessage);
                currMessage = currMessage->next;
            }
        }
        currBox = currBox->next;
    }
}

// int main(void)
// {
//     box_t *firstBox = NULL;

//     char *hello = "hello.txt";

//     addBox(&firstBox, hello);
//     addMessage(firstBox, "Hello how are you?");
//     addMessage(firstBox, "yeert");
//     addMessage(firstBox, "yup!");

//     addBox(&firstBox, "test1");

//     addBox(&firstBox, "test2");
//     addMessage(firstBox, "model car");
//     addMessage(firstBox, "trampoline");

//     dumpDetails(firstBox);

//     while (firstBox != NULL)
//     {
//         if (strcmp(firstBox->name, "hello.txt") == 0)
//         {
//             printf("found\n");
//         }
//         firstBox = firstBox->next;
//     }

//     return 0;
// }

// int createBox(int sock, box_t firstBox)
// {
// 	box_t thisBox = firstBox;

// 	char *boxName = malloc(sizeof(char) * 26);
// 	//	memset(filename, NULL, 26 * sizeof(char));

// 	char *client_message = "What is the name of the box you want to create?\n";
// 	char *boxCreated = "Box Created!\n";
// 	char *boxCreatedFail = "Box already exists!\n";
// 	char *wrongFormat = "Incorrect Format!\n";

// 	write(sock, client_message, strlen(client_message)) - 1;

// 	recv(sock, boxName, sizeof(boxName), 0);

// 	if (isdigit(boxName[0]) != 0)
// 	{
// 		printf("Incorrect Format!\n");
// 		write(sock, wrongFormat, strlen(wrongFormat)) - 1;
// 		return 0;
// 	}

// 	while (thisBox.next != NULL)
// 	{
// 		if (strcmp(thisBox.name, boxName) == 0)
// 		{

// 			printf("The box requested already exists!");
// 			write(sock, boxCreatedFail, strlen(boxCreatedFail) - 1);
// 			return NULL;
// 		}

// 		thisBox = thisBox.next;
// 	}

// 	box_t newBox;

// 	thisBox.next = newBox;
// 	newBox.name = boxName;
// 	newBox.next = NULL;

// 	write(sock, fileCreated, strlen(fileCreated)) - 1;

// 	return newBox;
// }