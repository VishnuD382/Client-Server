#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>

int main(int argc, char **argv)
{

    char exit[] = "quit";

    int sockfd;
    char buffer[1000]; //message
    char server_reply[2000];
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

    if (check == 0)
    {
        printf("Connected Succesfully!\n\n");

        while (strcmp(buffer, exit) != 0)
        {

            memset(server_reply, 0, sizeof(server_reply));
            printf("Enter a Command: ");
            scanf("%s", buffer);

            if (send(sockfd, buffer, strlen(buffer), 0) < 0)
            {
                printf("Error send \n");
                return 1;
            }

            if (recv(sockfd, server_reply, 2000, 0) < 0)
            {
                puts("Error recv\n");
                break;
            }

            printf("Server Reply: %s \n", server_reply);
        }
    }

    else if (check == -1)
        printf("Error connecting\n");

    close(sockfd);
    return 0;
}