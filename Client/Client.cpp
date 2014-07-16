#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

const char end_signal[2] = {'\x1a', '\0'};
const char quit_signal[2] = "q";

int main() {
    int clientSocket, server_port = 4444;
    char server_ip_addr[16];
    // create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        puts("cannot create socket!");
        exit(1);
    }
    char recv_buff[1024];
    char send_buff[1024];
    struct sockaddr_in clientAddr;
    clientAddr.sin_family      = AF_INET;
    clientAddr.sin_port        = htons(0);  // automatically choose a valid port
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(clientAddr.sin_zero), 0, 8);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_port        = htons(server_port);
    if ((bind(clientSocket, (struct sockaddr*) &clientAddr,
		sizeof(struct sockaddr))) == -1) {
        printf("Bad bind. errno : %d\n", errno);
        exit(1);
    }
    
    serverAddr.sin_addr.s_addr = inet_addr(server_ip_addr);
    memset(&(serverAddr.sin_zero), 0, 8);
    printf("enter the ip address of the server : ");
    gets(server_ip_addr);
    puts("start connecting...");
    if ((connect(clientSocket, (struct sockaddr*) &serverAddr,
        sizeof(struct sockaddr))) == -1) {
        printf("Connect fail! errno : %d\n", errno);
        exit(1);
    }
    puts("start receiving...");
    while (1) {
        recv(clientSocket, recv_buff, sizeof(recv_buff), 0);
        std::cout << recv_buff;
        gets(send_buff);
        send(clientSocket, send_buff, sizeof(send_buff), 0);
        if (strcmp(quit_signal, send_buff) == 0)
            break;
    }
    close(clientSocket);
    return 0;
}

