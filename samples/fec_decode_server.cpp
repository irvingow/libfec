//
// Created by lwj on 2019/10/17.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include "fec_decode.h"

int main(int argc, char *argv[]) {
    int server_sockfd = -1;
    struct sockaddr_in my_addr;
    char buf[2048] = {0};
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(8000);

    if ((server_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket error");
        return -1;
    }
    if (bind(server_sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind error");
        close(server_sockfd);
        return -1;
    }

    FecDecode fec_decoder(2000);
    while (true) {
        bzero(buf, sizeof(buf));
        int len = -1;
        if ((len = recvfrom(server_sockfd, buf, sizeof(buf), 0, 0, 0)) < 0) {
            perror("recvfrom error");
            break;
        }
        printf("recv data len:%d\n", len);
        auto ret = fec_decoder.Input(buf, len);
        while (ret > 0) {
            char *recv_buf = (char *)malloc(ret + 1);
            bzero(recv_buf, ret+1);
            ret = fec_decoder.Output(recv_buf, ret);
            if (ret < 0) {
                perror("failed to get decoded data from fec_decoder");
                break;
            }
            printf("data len:%d content:%s\n", len, recv_buf);
            free(recv_buf);
        }
        if(ret < 0){
            printf("error ret:%d\n", ret);
        }
    }
    return 0;
}