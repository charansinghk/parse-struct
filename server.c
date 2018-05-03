#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

typedef unsigned short int uint16_t;

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    printf("sizeof(unisigned short int) = %d\n",(int)sizeof(unsigned short int));
    printf("Listening on localhost, port 5000\n");
    uint16_t type = 5;
    uint16_t len = 10;
    char *data = "abcdefghij";
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        if (connfd <= 0)
          printf("accept failed: errono: %d\n",errno);
        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, (const void *)&type, sizeof(type));
        sleep(3);
        write(connfd, (const void *)&len, sizeof(len));
        sleep(3);
        write(connfd, data, strlen(data));
//        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
     }
}
