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
    char buf[128] = {0};
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(8000);

    if ((server_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
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
        if ((len = recvfrom(server_sockfd, buf, 128, 0, 0, 0)) < 0) {
            perror("recvfrom error");
            break;
        }
        auto ret = fec_decoder.Input(buf, strlen(buf));
        if (ret == 1) {
            std::vector<char *> data_pkgs;
            std::vector<int32_t> data_pkgs_length;
            ret = fec_decoder.Output(data_pkgs, data_pkgs_length);
            if (ret < 0) {
                perror("failed to get decoded data from fec_decoder");
                break;
            }
            printf("data_pkgs size:%d, data_pkgs_length size:%d\n",
                data_pkgs.size(), data_pkgs_length.size());
            for (int i = 0; i < data_pkgs.size(); ++i) {
                bzero(buf, sizeof(buf));
                memcpy(buf, data_pkgs[i], data_pkgs_length[i]);
                printf("%s\n", buf);
            }
        }
    }
    return 0;
}