//
// Created by lwj on 2019/10/16.
//
#include "common.h"

void write_u32(char * p,uint32_t l)
{
    *(unsigned char*)(p + 3) = (unsigned char)((l >>  0) & 0xff);
    *(unsigned char*)(p + 2) = (unsigned char)((l >>  8) & 0xff);
    *(unsigned char*)(p + 1) = (unsigned char)((l >> 16) & 0xff);
    *(unsigned char*)(p + 0) = (unsigned char)((l >> 24) & 0xff);
}
uint32_t read_u32(char * p)
{
    uint32_t res;
    res = *(const unsigned char*)(p + 0);
    res = *(const unsigned char*)(p + 1) + (res << 8);
    res = *(const unsigned char*)(p + 2) + (res << 8);
    res = *(const unsigned char*)(p + 3) + (res << 8);
    return res;
}

int64_t getnowtime_ms(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000;
}