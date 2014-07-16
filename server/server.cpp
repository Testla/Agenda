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
#include <sstream>
#include <pthread.h>
#include <sys/time.h>
#include "AgendaUI.h"

using std::stringstream;

pthread_mutex_t charge = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t onStart = PTHREAD_MUTEX_INITIALIZER;

// 建立UI,与之通信
stringstream *received = new stringstream, *tosend = new stringstream;
AgendaUI ui(received, tosend);

void delay(int ms);

void f() {
    ui.OperationLoop();
    return;
}

int main() {
	int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int port = 4444;
    char buffer[1024];
    
	// 建立Socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        puts("error creating socket!");
        exit(1);
    }
     
    // 定义端口和监听的地址
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_port        = htons(4444);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(serverAddr.sin_zero), 0, 8);
    int ret = bind(serverSocket, (struct sockaddr*) &serverAddr,
		     sizeof(struct sockaddr));
    if (ret == -1) {
	    printf("Bad bind. errno : %d\n", errno);
	    exit(1);
    } else {
        printf("socket binded successfully on port %d\n", port);
    }
    // 让serverSocket开始监听，客户队列长为5
    ret = listen(serverSocket, 5);
    if (ret == -1) {
	    printf("Bad listen. errno : %d\n", errno);
	    exit(1);
    }

    // 等待客户连接
    int sock;
    int clientAddrSize = sizeof(struct sockaddr_in);
    puts("starting UI");
    // 这边先发
    pthread_t id;
    pthread_mutex_lock(&onStart);
	pthread_create(&id, 0, (void* (*)(void*))f, 0);
	
    puts("waiting for connection");
    while ((sock = accept(serverSocket,
                          (struct sockaddr*) &clientAddr,
			              (socklen_t*) &clientAddrSize)) == -1);
    puts("accepted an connection");

    pthread_mutex_unlock(&onStart);
    while (1) {
        puts("waiting to send...");
        pthread_mutex_lock(&charge);
        delay(10);
        puts("start sending");
        send(sock, tosend->str().c_str(), strlen(tosend->str().c_str()) + 1, 0);
        tosend->str("");

        recv(sock, buffer, sizeof(buffer), 0);
        received->str(buffer);
        pthread_mutex_unlock(&charge);
        delay(100);
        if (strcmp(buffer, "q") == 0)
            break;
    }
    close(serverSocket);
    pthread_join(id, 0);
	return 0;
}


