#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "http.h"
#include "char_buf.h"

void terminate();

/* Sockets are here, because we have to use them inside terminate */
int s, sock_client;

int main()
{
    CharBuf *response = create_char_buf(4096);
    char request[1024];
    char terminated = 0;
    struct sockaddr_in serv_addr;

    signal(SIGINT, terminate);

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Failed to create socket ");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(1500);

    if (bind(s, (struct sockaddr *)(&serv_addr), sizeof(serv_addr)) == -1)
    {
        perror("Failed to bind socket: ");
        exit(1);
    }

    if (listen(s, 1) == -1)
    {
        perror("Failed to listen: ");
        exit(1);
    }

    printf("Server is listening on %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

    while (!terminated)
    {
        struct sockaddr_in client_addr;
        uint addr_len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));

        if ((sock_client = accept(s, (struct sockaddr *)(&client_addr), &addr_len)) == -1)
        {
            perror("Failed to accept connection: ");
            break;
        }

        while (1)
        {
            int read_qty = recv(sock_client, request, sizeof(request) - 1, 0);

            if (read_qty <= 0)
                break;

            int result = parse_http(request, read_qty, response);

            if (result == 0)
            {
                send(sock_client, response->buf, response->len, 0);
                clear_char_buf(response);
            }

            close(sock_client);
        }
    }

    terminate();
}

void terminate()
{
    if (close(sock_client) == -1)
    {
        perror("Failed to close client socket: ");
    }

    if (close(s) < 0)
    {
        perror("Failed to close socket ");
        exit(1);
    }

    printf("Server is off.\n");
    exit(0);
}