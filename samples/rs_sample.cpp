//
// Created by lwj on 2019/10/20.
//
#include <vector>
#include <cstdint>
#include <iostream>
#include <cstring>
#include "rs.h"
#include "common.h"

using namespace std;

int main(int argc, char *argv[]) {
    int i;
    char arr[15][39] =
        {
            "fhbweohfowehfowehf9023u905487odwusfios", "fdskjfhkasdhfkasdjfh92wfjoise", "fdshkjfhksajdhfaksjd", "fdshjkfhak",
            "origin 05", "origin 06", "origin 07", "origin 08", "origin 09", "origin 10",
            "origin 11", "origin 12", "origin 13", "origin 14", "origin 15",
        };
    char *data[15];
    for (i = 0; i < 15; i++) {
        data[i] = arr[i];
    }
    printf("--------origin data start------------\n");
    for (i = 0; i < 10; i++) {
        printf("<%s>\n", data[i]);
    }
    printf("--------origin data end------------\n");

    rs_encode2(10, 15, data, 38);
    printf("++++++++encoded data start++++++++++++\n");
    for (i = 0; i < 15; i++) {
        print_char_array_in_byte(data[i]);
    }
    printf("++++++++encoded data end++++++++++++\n");

    printf("@@@@@@@@trans data start@@@@@@@@@@@@\n");
    data[0] = NULL;
    data[1] = NULL;
    data[2] = NULL;
    data[10] = NULL;
    data[12] = NULL;
    for (i = 0; i < 15; i++) {
        if (data[i])
            print_char_array_in_byte(data[i]);
    }
    printf("@@@@@@@@trans data end@@@@@@@@@@@@@\n");

    int ret = rs_decode2(10, 15, data, 38);
    printf("ret:%d\n", ret);

    printf("########decoder data start##########\n");
    for (i = 0; i < 15; i++) {
        if (data[i])
            print_char_array_in_byte(data[i]);
        printf("<%s>\n", data[i]);
    }
    printf("########decoder data end##########\n");
    return 0;
}
