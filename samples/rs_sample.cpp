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
    if(argc < 3){
        printf("usage:%s data_pkgs_num redundant_pkgs_num(max_data_pkgs_num is 10, max_redundant_pkgs_num is 5)\n", argv[0]);
        return 0;
    }
    int data_pkgs_num = atoi(argv[1]);
    int redundant_pkgs_num  = atoi(argv[2]);
    int i;
    char arr[15][500] =
        {
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905487odwusfioshbweohfowehfowehf9023u905487odwusfios"
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwiluf",
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905487odwusfioshbweohfowehfowehf9023u905487odwusfios"
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwilufh1308hr73hrrfkjsdnfkjd",
            "fdskjfhkasdhfhbweohfowehfowehf9023u905487odwusfiosfkasdjfh92wfjoise",
            "fdsfhbweohfowehfowehf9023u905487odwusfioshfhbweohfowehfowehf9023u905487odwusfioskjfhksajdhfaksjd",
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905487odwusfioshbweohfowehfowehf9023u905487odwusfios"
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwilufh1308hr73hrrfkjsdnfkjd",
            "fdshjkfhak",
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwilufh1308hr73hrrfkjsdnfkjd",
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwilufh1308hr73hrrfkjsdnfkjd",
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwilufh1308hr73hrrfkjsdnfkjd"
            "origin 05", "origin 06", "origin 07", "origin 08", "origin 09", "origin 10",
            "h24yu9t34yhiuadshbiuwe9ty2e8rhsafsga[]fdgdfgd4fe+f4sda6fwe8fsd6a4x4312vsdf4we64fwe8r32r443y454y4dsfdf6sda"
        };
    char *data[15];
    for (i = 0; i < data_pkgs_num+redundant_pkgs_num; i++) {
        data[i] = arr[i];
    }
    printf("--------origin data start------------\n");
    for (i = 0; i < data_pkgs_num; i++) {
        printf("<%s>\n", data[i]);
    }
    printf("--------origin data end------------\n");

    rs_encode2(data_pkgs_num, data_pkgs_num+redundant_pkgs_num, data, 233);
//    printf("++++++++encoded data start++++++++++++\n");
//    for (i = 0; i < 3; i++) {
//        print_char_array_in_byte(data[i]);
//    }
//    printf("++++++++encoded data end++++++++++++\n");

//    printf("@@@@@@@@trans data start@@@@@@@@@@@@\n");
    data[0] = NULL;
    data[10] = NULL;
    data[14] = NULL;
//    for (i = 0; i < data_pkgs_num+redundant_pkgs_num; i++) {
//        if (data[i])
//            print_char_array_in_byte(data[i]);
//    }
//    printf("@@@@@@@@trans data end@@@@@@@@@@@@@\n");

    int ret = rs_decode2(data_pkgs_num, data_pkgs_num+redundant_pkgs_num, data, 233);
    printf("ret:%d\n", ret);

    printf("########decoder data start##########\n");
    for (i = 0; i < data_pkgs_num; i++) {
//        if (data[i])
//            print_char_array_in_byte(data[i]);
        printf("<%s>\n", data[i]);
    }
    printf("########decoder data end##########\n");
    return 0;
}
