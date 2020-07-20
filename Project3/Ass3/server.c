#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //socket
#include <unistd.h>
#include <time.h>
#include <pthread.h> //compile with lpthread

void *server_func(void *fd_pointer)
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char help[] = "help";

    //printf("Hello Server Handler \n");
    int sock = *(int *)fd_pointer;

    int read_size, write_size;
    char *message;
    static char client_message[2000];

    message = " \nHello Server Handler \n";

    static int send_once = 0;
    if (send_once < 1)
    {
        //write(sock, message, strlen(message));
        printf("Hello Server Handler \n");
        send_once++;
    }

    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("client_msg: %s\n", client_message);
        int clientCheckNum = 0;

        if (strcmp(client_message, "help") == 0)
            clientCheckNum = 1;
        else if (strcmp(client_message, "create") == 0)
            clientCheckNum = 2;
        else if (strcmp(client_message, "delete") == 0)
            clientCheckNum = 3;
        else if (strcmp(client_message, "open") == 0)
            clientCheckNum = 4;
        else if (strcmp(client_message, "close") == 0)
            clientCheckNum = 5;
        else if (strcmp(client_message, "next") == 0)
            clientCheckNum = 6;
        else if (strcmp(client_message, "put") == 0)
            clientCheckNum = 7;
        else if (strcmp(client_message, "quit") == 0)
            clientCheckNum = 8;

        memset(client_message, 0, sizeof(client_message));

        switch (clientCheckNum)
        {
        case 1:
            helpPrint(sock);
            break;
        case 2:
            createMessage(sock);
            break;
        case 3:
            deleteMessage(sock);
            break;
        case 4:
            openBox(sock);
            break;
        case 5:
            closeBox(sock);
            break;
        case 6:
            nextMessage(sock);
            break;
        case 7:
            putMessage(sock);
            break;
        case 8:
            write(sock, "Quitting!!!", 11);
            break;
        default:
            write(sock, "\nHuh? Give me something valid, please!\n", 37);
        }

        printf("%d%d %d %s \n", tm.tm_hour, tm.tm_min, tm.tm_mday, months[tm.tm_mon]);
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
    free(fd_pointer);

    return 0;
}

int main()
{
    char *boxLoc = "./boxes";
    mkdir(boxLoc, 0700);

    int listenfd, connfd, *new_sock;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1)
    {
        perror("Could not create Socket \n");
    }

    puts("Socket Created");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8888);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        return 1;
    }

    puts("bind success");
    listen(listenfd, 4);

    clilen = sizeof(cliaddr);

    while ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)))

    {
        puts("Connection accepted");

        pthread_t server_thread;
        new_sock = malloc(sizeof(int));
        *new_sock = connfd;
        pthread_create(&server_thread, NULL, server_func, (void *)new_sock);
    }

    if (connfd < 0)
    {
        perror("Accecpt Failed");
        return 1;
    }

    return 0;
}