#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define min(a,b) (a) < (b) ? a : b 
#define BUF_SIZE 256
int field_length[] = {2, 2, 10};

void emit(uint16_t type, uint16_t len, char *data) {
  int i = 0;
  printf("\n===========================================\n");
  printf("Type : %d , Length : %d\n", type, len);
  for (i = 0;i < len; i++) 
    printf("%c", (char)data[i]);
  printf("\n===========================================\n");
}

int parse_tlv(int fd) {
    unsigned char tmpBuf[BUF_SIZE];
    uint16_t type, length;
    int curIndx = 0;
    int bufIndx = 0, state = 0, prevIndx = 0, dataIndx = 0;
    char *data = NULL;
    void *field = NULL;
    while (1) {
        int tmpLen = read(fd, tmpBuf + bufIndx, sizeof(tmpBuf));
        if (tmpLen == 0){
            printf("Socket closed, total bytesRead = %d\n",curIndx);
            return 1;
        } else if (tmpLen < 0) {
            return errno;
        }
        printf("Socket read returned :%d bytes\n", tmpLen);
        bufIndx = (bufIndx + tmpLen) % BUF_SIZE;
        curIndx += tmpLen;
        // curIndx - prevIndx is the len of the buffer with "unread" data.
        while ( tmpLen > 0) {
           printf("Remaining : %d\n",tmpLen);
           int flen = 0;
           switch (state) {
               case 0:
                  field = (void*)&type;
                  flen = field_length[state];
                  break;
               case 1:
                  field = (void*)&length;
                  flen = field_length[state];
                  break;
               case 2:
                  if (data == NULL) {
                    data = (char*)malloc(sizeof(char) * length);
                  }
                  field = (void *)data + dataIndx;
                  /*
                   * case 1: if the entire varaible length payload is read in this iteration
                   *          then we will be done reading.
                   * case 2: if we got partial data of the payload, read only the currently
                   *          available number of bytes
                   */
                  flen = min(length, (curIndx - prevIndx));
                  dataIndx += flen;
                  break;
            }
            memcpy(field, tmpBuf+prevIndx, flen);
            prevIndx += flen;
            state++;
            tmpLen -= flen;
            if ( state > 1 && dataIndx >= length) {
              // Done reading one struct
              emit(type, length, data);
              state = 0;  
              // TODO: The reamining data in the buffer is for the next packet, hanle it.
              // If we have recv say only 1 bytes of the 'type' filed in the next packet
              // we need to somehow save this partial data;
            }
        } 
   }
   return 0;
}

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    int port = 80;

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port number>\n",argv[0]);
        return 1;
    } 
    
    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    port = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 
    printf("Connecting to %s:%d\n",argv[1],port);
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

   fprintf(stderr,"Succesfully connected to server %s : %d\n",argv[1],port);
    int i = 0;
    parse_tlv(sockfd);
/*
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        printf("Received %d bytes\n", n);
        for (i = 0; i < n; i++) {
          printf(" %d : %c\n",i, recvBuff[i]);
        }
        printf("\n");
        recvBuff[n] = 0;
        //printf("received %s\n",recvBuff);
    } 
*/
    return 0;
}


