#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>
#define MAXSIZE 2000U

int readInput(char *buffer)
{
    fgets(buffer, MAXSIZE, stdin);
    int readSize = strlen(buffer);

    if (readSize > 0 && buffer[readSize - 1] == '\n')
    {
        buffer[readSize - 1] = '\0';
    }
}

int main(int argc, char **argv)
{
    char exit[] = "quit";

    int sockfd;
    //char buffer[2000]; //message

    char *buffer = malloc(MAXSIZE * sizeof(char));

    char server_reply[MAXSIZE];
    ssize_t n;

    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // attempting to create socket

    if (sockfd == -1)
    {
        perror("Could not create socket");
    }
    printf("Created Socket \n");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));        // port number argv[2]
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr); // IPaddress argv[1]

    int check = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); // attempting to connect

    int first = 0;
    char init[] = "HELLO!";

    if (check == 0)
    {
        printf("Connected Succesfully!\n\n");

        while (strcmp(buffer, exit) != 0)
        {
            memset(server_reply, 0, sizeof(server_reply));

            if (first == 0) // first message send
            {
                send(sockfd, init, strlen(init), 0);
                recv(sockfd, server_reply, MAXSIZE, 0);

                if (strcmp(server_reply, "HELLO DUMBv0 ready!") != 0)
                {

                    printf("Not the correct server message!\nQuitting!\n");
                    return -1;
                }
                first = 1;
                printf("%s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }

            printf("\nEnter a Command: ");

            readInput(buffer);

            if (send(sockfd, buffer, strlen(buffer), 0) < 0)
            {
                printf("Error send \n");
                return 1;
            }

            if (recv(sockfd, server_reply, MAXSIZE, 0) < 0)
            {
                puts("Error recv\n");
                break;
            }

            printf("%s", server_reply);

            if (strcmp(server_reply, "What is the file name you want to create?\n") == 0) // create
            {
                printf("\nEnter File name to Create:");
                readInput(buffer);
                printf("buffer: %s\n", buffer);

                send(sockfd, buffer, strlen(buffer), 0);

                memset(server_reply, 0, sizeof(server_reply));
                recv(sockfd, server_reply, MAXSIZE, 0);

                printf("%s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }

            else if (strcmp(server_reply, "What is the file name you want to delete?\n") == 0) // delete
            {
                printf("\nEnter File name to Delete: ");

                readInput(buffer);

                send(sockfd, buffer, strlen(buffer), 0);
                memset(server_reply, 0, sizeof(server_reply));
                recv(sockfd, server_reply, MAXSIZE, 0);
                printf("%s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }

            else if (strcmp(server_reply, "File you would like to Open\n") == 0) // open
            {
                printf("\nEnter File name to Open: ");

                readInput(buffer);

                send(sockfd, buffer, strlen(buffer), 0);
                memset(server_reply, 0, sizeof(server_reply));
                recv(sockfd, server_reply, MAXSIZE, 0);

                printf("%s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }

            else if (strcmp(server_reply, "File you would like to Close?\n") == 0) // close
            {
                printf("\nEnter File name to Close: ");
                readInput(buffer);
                send(sockfd, buffer, strlen(buffer), 0);
                memset(server_reply, 0, sizeof(server_reply));
                recv(sockfd, server_reply, MAXSIZE, 0);
                printf("%s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }

            else if (strcmp(server_reply, "Enter your message: \n") == 0) // putmessage
            {
                printf("\nEnter Message: ");

                //scanf("%s", buffer);
                readInput(buffer);
                //gets(buffer);

                printf("%s\n", buffer);
                send(sockfd, buffer, strlen(buffer), 0);
                memset(server_reply, 0, sizeof(server_reply));
                recv(sockfd, server_reply, MAXSIZE, 0);
                printf("%s\n", server_reply);
                memset(server_reply, 0, sizeof(server_reply));
            }
            else if (strcmp(server_reply, "Huh? Give me something valid, please\n") == 0)
            {
                printf("WHat?\n");
            }
        }
    }

    else if (check == -1)
        printf("Error connecting\n");

    close(sockfd);

    return 0;
}