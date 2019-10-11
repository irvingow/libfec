//
// Created by lwj on 2019/10/11.
//

#include <cstring>
#include <algorithm>
#include "fec_encode.h"
#include "random_generator.h"
#include "common.h"

FecEncode::FecEncode(const int32_t &data_pkg_num, const int32_t &redundant_pkg_num)
    : cur_data_pkgs_num_(0),
      max_data_pkg_length_(0),
      ready_for_fec_output_(false),
      data_pkg_num_(data_pkg_num),
      redundant_pkg_num_(redundant_pkg_num) {
    RandomNumberGenerator *rg = RandomNumberGenerator::GetInstance();
    auto ret = rg->GetRandomNumber(seq);
    if (ret < 0) {
        seq = 0;
    }
    data_pkgs_.resize(data_pkg_num + redundant_pkg_num);
    for(char *pkg : data_pkgs_){
        pkg = nullptr;
    }
}

int32_t FecEncode::Input(char *input_data_pkg, int32_t length) {
    std::lock_guard<std::mutex> lck(data_pkgs_mutex_);
    if (cur_data_pkgs_num_ == 0 || cur_data_pkgs_num_ == data_pkg_num_) {
        ready_for_fec_output_ = false;
        max_data_pkg_length_ = 0;
        FreeDataPkgs();
    }
    length += fec_encode_head_length_;
    max_data_pkg_length_ = std::max(length, static_cast<int32_t >(max_data_pkg_length_));
    data_pkgs_[cur_data_pkgs_num_] = (char *) malloc((length + 1) * sizeof(char));
    bzero(data_pkgs_[cur_data_pkgs_num_], length + 1);
    write_u32(data_pkgs_[cur_data_pkgs_num_], seq);
    data_pkgs_[cur_data_pkgs_num_][4] = (unsigned char)data_pkg_num_;
    data_pkgs_[cur_data_pkgs_num_][5] = (unsigned char)redundant_pkg_num_;
    data_pkgs_[cur_data_pkgs_num_][6] = (unsigned char)cur_data_pkgs_num_;
    memcpy(data_pkgs_[cur_data_pkgs_num_] + fec_encode_head_length_, input_data_pkg, length - fec_encode_head_length_);
    cur_data_pkgs_num_++;
    if (cur_data_pkgs_num_ == data_pkg_num_) {
        char **data = nullptr;
        data = (char **) malloc((data_pkg_num_ + redundant_pkg_num_) * sizeof(char *));
        for (int32_t i = 0; i < data_pkg_num_ + redundant_pkg_num_; ++i) {
            if (i < data_pkg_num_) {
                data[i] = data_pkgs_[i];
            } else {
                data[i] = (char *) malloc((max_data_pkg_length_ + 1) * sizeof(char));
                bzero(data[i], (max_data_pkg_length_ + 1) * sizeof(char));
            }
        }
        rs_encode2(data_pkg_num_, data_pkg_num_ + redundant_pkg_num_, data, max_data_pkg_length_);
        for (int32_t i = 0; i < data_pkg_num_ + redundant_pkg_num_; ++i) {
            data_pkgs_[i] = data[i];
        }
        free(data);
        ready_for_fec_output_ = true;
        seq++;
        return 1;
    }
    return 0;
}

int32_t FecEncode::Output(std::vector<char *> &data_pkgs) {
    if (!ready_for_fec_output_) {
        return -1;
    }
    data_pkgs = data_pkgs_;
    cur_data_pkgs_num_ = 0;
    return 0;
}

void FecEncode::FreeDataPkgs() {
    for (char *data_pkg : data_pkgs_) {
        free(data_pkg);
        data_pkg = nullptr;
    }
}

