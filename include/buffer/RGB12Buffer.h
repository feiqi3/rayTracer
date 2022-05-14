/*
 * @Author: feiqi3
 * @Date: 2022-05-12 11:10:47
 * @LastEditTime: 2022-05-14 08:21:35
 * @LastEditors: feiqi3
 * @Description: |---a buffer to save float3,or vec3 in glsl :)---|
 * @FilePath: \rayTracer\include\buffer\RGB12Buffer.h
 * ->blog: feiqi3.cn <-
 */

#ifndef RGB16BUUFFER_H
#define RGB16BUUFFER_H

#include "../math/vector.h"
#include "Buffer.h"

class RGB12 : public buffer {
public:
  RGB12(int _x, int _y) : buffer(_x, _y) {}
  ~RGB12();

  bool make_buffer() override;
  void del_buffer() override;
  vec3 sampler(float x, float y) const override;
  void writeBuffer(float x, float y, const vec3 in) override;

protected:
  vec3 **bufferMap;
};

inline RGB12::~RGB12() {
  if (init == true) {
    del_buffer();
  }
}

inline bool RGB12::make_buffer() {
  del_buffer();
  bufferMap = new vec3 *[y];
  for (size_t i = 0; i < x; i++) {
    bufferMap[i] = new vec3[x];
  }
  init = true;
  return true;
}

inline void RGB12::del_buffer() {
  if (init == false) {
    return;
  }

  for (int i = 0; i < x; i++) {
    delete[] bufferMap[i];
  }

  delete[] bufferMap;
  init = false;
}

inline vec3 RGB12::sampler(float _x, float _y) const {
  if (!init) {
    return vec3(0, 0, 0);
  }
  int sample_x = _x * x;
  int sample_y = _y * y;
  //clamp sample x,y to [0,x);
  sample_x = (sample_x > x - 1 ? x - 1 : sample_x) < 0 ? 0 : sample_x;
  sample_y = (sample_y > y - 1 ? y - 1 : sample_y) < 0 ? 0 : sample_y;
  return bufferMap[sample_x][sample_y];
}

inline void RGB12::writeBuffer(float _x, float _y, const vec3 _in) {
  bufferMap[(int)(_x * x)][(int)(_y * y)] = _in;
}
#endif