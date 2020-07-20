#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //socket
#include <unistd.h>
#include <time.h>
#include <pthread.h> //compile with lpthread

#include "LL.c"

typedef struct args
{
    //pthread_mutex_t lock;
    int *sock;
    box_t *box;

} args_t;

int server_func(void *fd_pointer)
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    args_t *mem = (args_t *)fd_pointer;

    int read_size, write_size;
    char *message;
    static char client_message[2000];
    int openBoxFD = 0;

    message = "HELLO DUMBv0 ready!";

    printf("box: %p\n", (void *)&(mem->box));
    printf("sock: %d\n", *mem->sock);

    static int send_once = 0;

    box_t *curr = NULL;
    box_t *temp1 = NULL;

    int sock = 0;
    sock = *mem->sock;

    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        printf("client_msg: %s\n", client_message);

        int clientCheckNum = 0;
        if (strcmp(client_message, "HELLO!") == 0)
        {
            write(sock, message, strlen(message));
            memset(client_message, 0, sizeof(client_message));
            continue;
        }

        else if (strcmp(client_message, "help") == 0)
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
            temp1 = mem->box->next;
            while (temp1 != NULL)
            {
                printf("%s\n", temp1->name);
                temp1 = temp1->next;
            }
            break;
        case 2:
            if (mem->box->next == NULL)
            {
                mem->box->next = createBox(sock, NULL);
            }
            else
                addBox(mem->box->next, sock);
            break;
        case 3:
            mem->box->next = removeBox(mem->box->next, sock);
            break;
        case 4:
            if (curr != NULL)
            {
                if (closeBox(sock, curr))
                    curr = NULL;
                else
                    break;
            }
            box_t *temp = openBox(sock, mem->box->next);
            if (temp != NULL)
            {
                curr = temp;
            }
            break;
        case 5:
            if (closeBox(sock, mem->box->next))
            {
                curr = NULL;
            }
            break;
        case 6:
            nextMessage(sock, curr);
            break;
        case 7:
            putMessage(sock, curr);
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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("INVALID INPUT\n");
        return -1;
    }

    int port = atoi(argv[1]);

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
    servaddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        return 1;
    }

    puts("bind success");
    listen(listenfd, 4);

    clilen = sizeof(cliaddr);
    box_t *box = malloc(sizeof(box_t));
    box->next = NULL;
    box->firstMessage = NULL;

    while ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)))

    {
        args_t arg0;
        puts("Connection accepted");

        pthread_t server_thread;
        new_sock = malloc(sizeof(int));
        *new_sock = connfd;

        arg0.sock = new_sock;
        arg0.box = box;

        printf("sock: %d\n", *arg0.sock);

        pthread_create(&server_thread, NULL, (void *)server_func, (void *)&arg0);
    }

    if (connfd < 0)
    {
        perror("Accecpt Failed");
        return 1;
    }

    return 0;
}