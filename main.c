#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER 1024

int main(int argc, char *argv[]){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER];
    ssize_t bytes_received;

    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        fprintf(stderr, "Usage: %s -p port\n", argv[0]);
        exit(1);
    }

    int port_num = atoi(argv[2]);
    if(port_num <= 1024 || port_num > 65535){
        fprintf(stderr, "Port must be between 1024 and 65535\n");
        exit(EXIT_FAILURE);
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_num);

    if (listen(server_fd, 1) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", port_num);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;  
        }

        printf("New connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        while ((bytes_received = recv(client_fd, buffer, BUFFER - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';  

            if (send(client_fd, buffer, bytes_received, 0) == -1) {
                perror("Send failed");
                break;
            }
        }

        if (bytes_received == -1) {
            perror("Receive failed");
        } else {
            printf("Client disconnected\n");
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
