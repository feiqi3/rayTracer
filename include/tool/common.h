/*
 * @Author: feiqi3
 * @Date: 2022-02-02 12:09:27
 * @LastEditTime: 2022-03-29 13:47:54
 * @LastEditors: feiqi3
 * @Description: |common untilitise|
 * @FilePath: \rayTracer\include\tool\common.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <stdlib.h>
#include <cmath>

using std::make_shared;
using std::shared_ptr;
using std::lerp;
constexpr double pi = 3.14159265358979;
constexpr double doublePi =2 * 3.14159265358979;
constexpr double inv_4pi = 1./(4*pi);
constexpr double inv_2pi = 1./(2*pi);
constexpr double inv_pi = 1./(pi);

const double Infinity = std::numeric_limits<double>::infinity();

template<typename T>
T pow2(T t){
  return t*t;
}

inline float InvSqrt(float x) {
  float xhalf = 0.5f * x;
  int i = *(int *)&x;
  i = 0x5f3759df - (i >> 1); // 计算第一个*似根
  x = *(float *)&i;
  x = x * (1.5f - xhalf * x * x); // 牛顿迭代法
  return 1 / x;
}

inline float f_sqrt(float x) { return 1 / InvSqrt(x); }

inline double d2radian(double degree) { return (degree * pi * 0.00555555556); }

inline double rand_d() {
  return rand() / (RAND_MAX + 1.0); // make it in [0,1)
}

inline double rand_d(double min, double max) {
  return min + (max - min) * rand_d();
}

inline int rand_i(int min,int max){
  return static_cast<int>(rand_d(min, max));
}

template <typename T> inline T clamp(T x, T min, T max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}



#endif