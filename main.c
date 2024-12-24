#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void process_req(char *);

int main() {
    int s, sock_client;
    char terminated = 0;
    char host_name[256];
    struct sockaddr_in serv_addr;

    gethostname(host_name, sizeof(host_name));
    printf("Starting server on %s\n", host_name);

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed to create socket ");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(1500);

    printf("Binding socket to the port...\n");
    if (bind(s, (struct sockaddr *)(&serv_addr), sizeof(serv_addr)) == -1) {
        perror("Failed to bind socket: ");
        exit(1);
    }

    printf("Starting to listen...\n");
    if (listen(s, 1) == -1) {
        perror("Failed to listen: ");
        exit(1);
    }

    printf("Server is listening on %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));

    while (!terminated) {
        printf("Waiting for connections...\n");

        struct sockaddr_in client_addr;
        uint addr_len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));
        
        if ((sock_client = accept(s, (struct sockaddr*)(&client_addr), &addr_len)) == -1) {
            perror("Failed to accept connection: ");
            break;
        }

        char* response = "Hello from server\n";

        send(sock_client, response, strlen(response), 0);

        char received_buffer[1024];

        while (1) {
            int read_qty = recv(sock_client, received_buffer, sizeof(received_buffer) - 1, 0);

            if (read_qty <= 0)
                break;

            received_buffer[read_qty] = 0;

            process_req(received_buffer);

            break;
        }

        if (close(sock_client) == -1) {
            perror("Failed to close client socket: ");
            break;
        }

        printf("Client disconnected.\n");

        terminated = 1;
    }

    printf("Server is shutting down...\n");

    if (close(s) < 0) {
        perror("Failed to close socket ");
        exit(1);
    }

    printf("Done.\n");
}

void process_req(char* req) {
    char* start = req;
    for (; *req != 0; req++) {
        if (*req == '\n' || *req == '\r') {
            *req = 0;
            break;
        }
    }

    if (*start == 0) {
        printf("Received empty req\n");
        return;
    }

    printf("Received req: %s\n", start);
}