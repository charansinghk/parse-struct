#include<iostream>
#include<socket>

using namespace std;


#define BUF_SIZE 256

void emit(uint16_t type, uint16_t length, void* data) {
  //do something with the data provided by parse_tlv …
}

int field_length[] = {2, 2, -1};

int parse_tlv(int socket_fd) {
    uint16_t type, length;
    void *data;
    unsigned char tmpBuf[BUF_SIZE];
    int bufIndx = 0;
    char *data = NULL;
    int state = 0;
    while (true) {
        tmpLen = recv(fd, tmpBuf + bufIndx, sizeof(tmpBuf));
        if (tmpLen == 0){
            // socket closed
            return 1;
        } else if (tmpLen < 0) {
            return errno;
        }        
        bufIndx = (bufIndx + tmpLen) % BUF_SIZE;
        curLen += tmpLen;
        if (curLen > field_length[iState]) {
           switch (iState) {
               case 0:
                  field = &type;
                  break;
               case 1:
                  field = &length;
               case 2:
                  
                  
            }
            memcpy(field, tmpBuf+prevIndx, field_length[iState]);
            iState++;
        } else {
            printf("Reading more data");
        }
   }
            if (data == NULL) {
                data = (char*)malloc(length);
            }
            memcpy(data+curIndx,tmpBuf+bufIndx-tmpLen,tmpLen);
            curIndx += tmpLen;
            if (  >= length) {
                // completed reading “length” bytes
                emit(type, length, data);
                free(data);
                data = NULL;
                curIndx = 0;
                state = 0;
            }    
        }
        return 0;
    
}


