//
// Created by lwj on 2019/10/11.
//

#ifndef LIBFEC_RANDOM_GENERATOR_H
#define LIBFEC_RANDOM_GENERATOR_H

#include <boost/noncopyable.hpp>
#include <cstdint>
///boost单例模式实现
class RandomNumberGenerator : public boost::noncopyable {
 public:
  static RandomNumberGenerator *GetInstance();
  int32_t GetRandomNumber(uint32_t &random_number);
  int32_t GetRandomNumberNonZero(uint32_t &random_number);

 protected:
  struct Obj_Creator {
    Obj_Creator();
  };
  static Obj_Creator obj_creator_;
  RandomNumberGenerator();
  ~RandomNumberGenerator();
  int32_t random_number_fd_;
};

#endif //LIBFEC_RANDOM_GENERATOR_H
