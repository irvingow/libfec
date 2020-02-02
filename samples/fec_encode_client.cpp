//
// Created by lwj on 2019/10/17.
//

#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <common.h>
#include "fec_encode.h"

int32_t AddEvent2Epoll(const int32_t &epoll_fd, const int32_t &fd, const uint32_t &events) {
    struct epoll_event ev = {0};
    ev.events = events;
    ev.data.fd = fd;
    auto ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if (ret != 0)
        return -1;
    return 0;
}

int main(int argc, char *argv[]) {
    int client_sockfd = -1;
    struct sockaddr_in remote_addr = {0};
    char buf[2048] = {0}, send_buf[2048] = {0};
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    remote_addr.sin_port = htons(8000);

    if ((client_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket error");
        return -1;
    }
    int socklen = sizeof(struct sockaddr);
    FecEncode fec_encoder(4, 2, 30);
    int32_t epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
        return -2;
    ///创建timer_fd,提醒间隔为50ms,将其加入到epoll监听池中
    struct itimerspec temp = {0, 0, 0, 0};
    temp.it_value.tv_sec = 1;
    temp.it_value.tv_nsec = 0;
    temp.it_interval.tv_sec = 1;
    temp.it_interval.tv_nsec = 0;
    int32_t timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timerfd == -1) {
        printf("error call timerfd_create error:%s\n", strerror(errno));
        return -1;
    }
    if (timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &temp, nullptr) == -1) {
        printf("error call timerfd_settime error:%s\n", strerror(errno));
        return -1;
    }
    auto ret = AddEvent2Epoll(epoll_fd, timerfd, EPOLLIN);
    if (ret < 0) {
        printf("error call AddEvent2Epoll\n");
        return ret;
    }
    ///create listener to standard input
    ret = AddEvent2Epoll(epoll_fd, STDIN_FILENO, EPOLLIN);
    if (ret < 0) {
        printf("error call AddEvent2Epoll\n");
        return ret;
    }
    struct epoll_event events[5];
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, 5, -1);
        if (nfds < 0 && errno != EINTR) {
            printf("error call epoll_wait error:%s\n", strerror(errno));
            break;
        }
        for (int index = 0; index < nfds; ++index) {
            if (events[index].data.fd == STDIN_FILENO) {
                scanf("%s", buf);
                if (!strcmp(buf, "quit")) {
                    break;
                }
                ret = fec_encoder.Input(buf, strlen(buf));
                if (ret < 0) {
                    printf("failed to call fec_encoder input\n");
                    continue;
                } else if (ret == 1) {
                    std::vector<char *> data_pkgs;
                    std::vector<int32_t> data_pkgs_length;
                    ret = fec_encoder.Output(data_pkgs, data_pkgs_length);
                    if (ret < 0) {
                        perror(" Output data failed");
                        continue;
                    }
                    for (int i = 0; i < data_pkgs.size(); ++i) {
                        if ((i % 5) == 0)
                            continue;
                        bzero(send_buf, sizeof(send_buf));
                        memcpy(send_buf, data_pkgs[i], data_pkgs_length[i]);
                        ///add 11 is to skip the fec_encode_header, easier to debug
                        printf("send_buf:%s length:%d\n", send_buf + 11, data_pkgs_length[i] - 11);
                        ret = sendto(client_sockfd, send_buf, data_pkgs_length[i], 0, (struct sockaddr *) &remote_addr,
                                     socklen);
                        if (ret < 0) {
                            printf("sendto error:%s\n", strerror(errno));
                            break;
                        }
                    }
                }
            } else if (events[index].data.fd == timerfd) {
                ret = fec_encoder.FecEncodeUpdateTime(getnowtime_ms());
                if (ret < 0)
                    printf("error call fec_encoder FecEncodeUpdateTime\n");
                else if (ret > 0) {
                    std::vector<char *> data_pkgs;
                    std::vector<int32_t> data_pkgs_length;
                    ret = fec_encoder.FlushUnEncodedData(data_pkgs, data_pkgs_length);
                    if (ret < 0) {
                        perror(" error call fec_encoder.FlushUnEncodedData\n");
                        continue;
                    }
                    for (int i = 0; i < data_pkgs.size(); ++i) {
                        bzero(send_buf, sizeof(send_buf));
                        memcpy(send_buf, data_pkgs[i], data_pkgs_length[i]);
                        ///add 4 is to skip the unique header, easier to debug
                        printf("send_buf:%s length:%d\n", send_buf + 4, data_pkgs_length[i] - 4);
                        ret = sendto(client_sockfd, send_buf, data_pkgs_length[i], 0, (struct sockaddr *) &remote_addr,
                                     socklen);
                        if (ret < 0) {
                            printf("sendto error:%s\n", strerror(errno));
                            break;
                        }
                    }
                }
            }
        }
    }
    close(epoll_fd);
    close(timerfd);
    close(client_sockfd);
    return 0;
}