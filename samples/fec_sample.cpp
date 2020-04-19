//
// Created by lwj on 2019/10/11.
//
#include <glog/logging.h>
#include "fec_encode.h"
#include "fec_decode.h"
#include <chrono>
#include <algorithm>

using std::chrono::high_resolution_clock;
using std::chrono::microseconds ;

int unit_test() {
    FecEncode fec_encode(10, 5);
    const int32_t fec_encode_header_len = 11;
    int i, maxPackageLen = 0;
    char arr[15][600] =
        {
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdwehf9023u905487odwusfioshbwkhjkxbkjcvbxczkjveveohfowehfowehf9023u905487odwusfios"
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkj3u905487odwusfioscvbxczkjvevwehf902hbweohfowehfowehf9023u905487odwusfios"
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905487odwusfioshbweohfowehfowehf9023u905487odwusfios"
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905487odwusfioshbweohfowehfowehf9023u905487odwusfios"
            "fsdjhfksjdjkskjnjcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5weru82fjfmclkasdjfaksdhfaslkjfhwilufh1308hr73hrrfkjsdnfkjd",
            "ffhbweohfowehfo38490hs7odwusfioshbweohfowehfowehfiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905489023u905487odwusfios"
            "ffhbweohfowehfo3849xbkjcvbxczkjvevwehf9023u905487odwusfiosh0hsiudahf8awe9hfsdkhjkbweohfowehfowehf9023u905487odwusfios"
            "ffhbweohfowehfo38490hsiudahf8awe9hfsdkhjkxbkjcvbxczkjvevwehf9023u905487odwusfioshbweohfowehfowehf9023u905487odwusfios"
            "fsdjhfksjdjkskjnweru82fjfmclkasdjfaksdhfasljcxznvwio320urjngdakdsf4s5d3f4r5te8g4564a5kjfhwilufh1308hr73hrrfkjsdnfkjd",
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
    printf("--------origin data start------------\n");
    for (i = 0; i < 15; i++) {
        printf("<%s>\n", arr[i]);
    }
    printf("--------origin data end------------\n");

    int ret = 0;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    for (int index = 0; index < 15; ++index) {
        auto slen = strlen(arr[index]);
        maxPackageLen = std::max(maxPackageLen, static_cast<int>(slen));
        ret = fec_encode.Input(arr[index], slen);
        printf("package#%d length:%d\n", i, slen);
        if (ret == 1) {
//            LOG(INFO) << "fec_encode input ret success index:" << index;
            break;
        }
    }
    high_resolution_clock::time_point  end = high_resolution_clock::now();
    microseconds timeInterval = std::chrono::duration_cast<microseconds>(end - start);
    printf("computation consumes %ld us\n", timeInterval.count());
    std::vector<char *> res;
    std::vector<int32_t> res_length;
    ret = fec_encode.Output(res, res_length);
    if (ret != 0) {
        LOG(ERROR) << "failed to get fec_encoded data";
        return -1;
    }
//    rs_encode2(10, 15, data, 9);
//    printf("++++++++encoded data start++++++++++++\n");
//    for (i = 0; i < 15; i++) {
//        ///加11是为了跳过fec所加的包头
//        printf("<%s>\n", res[i] + fec_encode_header_len);
//    }
//    printf("++++++++encoded data end++++++++++++\n");

    char *data[15];
    for (i = 0; i < 15; ++i) {
        data[i] = (char *) malloc((res_length[i] + 1) * sizeof(char));
        bzero(data[i], (res_length[i] + 1) * sizeof(char));
        memcpy(data[i], res[i], res_length[i]);
    }
//    for(const auto length : res_length){
//        printf("length:%d ", length);
//    }
//    printf("\n");
//    printf("@@@@@@@@trans data start@@@@@@@@@@@@\n");
    free(data[0]);
    data[0] = nullptr;
    free(data[1]);
    data[1] = nullptr;
    free(data[2]);
    data[2] = nullptr;
    free(data[10]);
    data[10] = nullptr;
    free(data[12]);
    data[12] = nullptr;
//    for (i = 0; i < 15; i++) {
//        if (data[i] == nullptr) {
//            printf("<nullptr>\n");
//            continue;
//        }
//        ///加11是为了跳过fec所加的包头
//        printf("<%s>\n", data[i] + fec_encode_header_len);
//    }
//    printf("@@@@@@@@trans data end@@@@@@@@@@@@@\n");

    for (int i = 0; i < 15; ++i) {
        if (data[i] != nullptr)
            ///加11是为了跳过fec所加的包头
            data[i] += fec_encode_header_len;
    }
    rs_decode2(10,15,data, maxPackageLen);
    for (i = 0; i < 10; i++) {
        if (data[i] == nullptr) {
            printf("<nullptr>\n");
            continue;
        }
        printf("<%s>\n", data[i]);
    }
    return 0;

    /*
    FecDecode fec_decoder(2000);
    for (int32_t i = 0; i < 15; ++i) {
        if (data[i] == nullptr)
            continue;
        ret = fec_decoder.Input(data[i], strlen(data[i]));
        LOG(INFO) << "Input data to fec_decoder index:" << i << " length:" << strlen(data[i]);
        if (ret == 1) {
            LOG(INFO) << "data pkg number is enough for output i:" << i;
        }
    }

    std::vector<char *> decode_data_pkgs;
    std::vector<int32_t> decode_data_pkgs_length;
    ret = fec_decoder.Output(decode_data_pkgs, decode_data_pkgs_length);
    if (ret < 0) {
        LOG(ERROR) << "cannot get decoded data";
        return -1;
    }
    if (ret > 0) {
        LOG(INFO) << "more decoded data is ready for output in decoder ret:" << ret;
    }
    for (i = 0; i < 15; ++i) {
        free(data[i]);
        if (i < 10) {
            data[i] = (char *) malloc((decode_data_pkgs_length[i] + 1) * sizeof(char));
            bzero(data[i], (decode_data_pkgs_length[i] + 1) * sizeof(char));
            memcpy(data[i], decode_data_pkgs[i], decode_data_pkgs_length[i]);
        } else
            data[i] = nullptr;
    }

//    注意这里的size必须是16,因为数据包大小是9而fec的包头大小是7
//    ret = rs_decode2(10, 15, data, 16);
//    printf("ret:%d\n", ret);

    printf("########decoder data start##########\n");
    for (i = 0; i < 15; i++) {
        if (data[i] == nullptr)
            continue;
        printf("<%s>\n", data[i]);
    }
    printf("########decoder data end##########\n");
    return 0;
    */
}

int main(int argc, const char *argv[]) {
    google::InitGoogleLogging("INFO");
    FLAGS_logtostderr = true;
    unit_test();
    return 0;
}
