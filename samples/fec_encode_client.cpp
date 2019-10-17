//
// Created by lwj on 2019/10/17.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include "fec_encode.h"

int main(int argc, char *argv[]){
    int client_sockfd = -1;
    struct sockaddr_in remote_addr;
    char buf[128] = {0}, send_buf[128] = {0};
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote_addr.sin_port = htons(8000);

    if((client_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket error");
        return -1;
    }
    int socklen = sizeof(struct sockaddr);
    FecEncode fec_encoder(4, 2);
    while(true){
        printf("Enter string to send:");
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        if(!strcmp(buf, "quit")){
            break;
        }
        auto ret = fec_encoder.Input(buf, strlen(buf));
        if(ret == 1){
            std::vector<char*> data_pkgs;
            std::vector<int32_t > data_pkgs_length;
            ret = fec_encoder.Output(data_pkgs, data_pkgs_length);
            if(ret < 0){
                perror(" Output data failed");
                break;
            }
            for(int i = 0; i < 6; ++i){
                if((i % 5) == 0)
                    continue;
                bzero(send_buf, sizeof(send_buf));
                memcpy(send_buf, data_pkgs[i], data_pkgs_length[i]);
                printf("send_buf:%s\n", send_buf);
                ret = sendto(client_sockfd, send_buf, strlen(send_buf),0, (struct sockaddr*)&remote_addr,
                    socklen);
                if(ret < 0){
                    perror("sendto error");
                    break;
                }
            }
        }
    }
    close(client_sockfd);
    return 0;
}