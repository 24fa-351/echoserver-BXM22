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

    if (argc != 3 || strcmp(argv[1], "-p") != 0) {
        fprintf(stderr, "Usage: %s -p port\n", argv[0]);
        exit(1);
    }

    //port validation
    int port_num = atoi(argv[2]);
    if(port_num <= 1025 || port_num > 65535){
        fprintf(stderr, "Port must be between 1024 and 65535\n");
        exit(EXIT_FAILURE);
    }






    return 0;
}