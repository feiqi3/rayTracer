/*
 * @Author: feiqi3
 * @Date: 2022-05-12 11:10:47
 * @LastEditTime: 2022-05-18 14:13:26
 * @LastEditors: feiqi3
 * @Description: |---a buffer to save float3,or vec3 in glsl :)---|
 * @FilePath: \rayTracer\include\buffer\RGB12Buffer.h
 * ->blog: feiqi3.cn <-
 */

#ifndef RGB12BUUFFER_H
#define RGB12BUUFFER_H

#include "Buffer.h"
#include "Macro.h"
#include "math/vector.h"
#include "tool/common.h"
#include <cmath>
#include <string>

class RGB12 : public buffer {
public:
  RGB12(int _x, int _y) : buffer(_x, _y) { make_buffer(); }
  RGB12(unsigned char *_data, int _x, int _y)
      : buffer(_x, _y), _buffer_map(_data) {
    if (_data) {
      init = true;
      load_from_file = true;
    }
  }
  ~RGB12();
  bool make_buffer() override;
  void del_buffer() override;
  void writeBufferf(float x, float y, const vec3 in) override;
  void writeBuffer(int x, int y, const vec3 in) override;
  virtual void *getData() const override;
  virtual vec3 sampler(double u, double v) const override;
  void setInterpolation(bool flag) { linearInterpolation = flag; }
protected:
  vec3 sampler(int u, int v) const override;
  unsigned char *_buffer_map;
  vec3 BilinearSampler(float sampler_x,float sample_y)const;
private:
  bool load_from_file;
  static constexpr double colorScale = 1. / 255;
  int linearInterpolation;
};

inline RGB12::~RGB12() {
  if (init == true && _buffer_map != nullptr) {
    del_buffer();
  }
}

inline bool RGB12::make_buffer() {
  del_buffer();
  _buffer_map = new unsigned char[x * y * 3];
  init = true;
  return true;
}

inline void RGB12::del_buffer() {
  if (init == false || _buffer_map == nullptr) {
    return;
  }
  if (!load_from_file) {
    delete[] _buffer_map;
    _buffer_map = nullptr;

  } else {
    free(_buffer_map);
    _buffer_map = nullptr;
    load_from_file = false;
  }
  init = false;
}

inline void RGB12::writeBufferf(float _x, float _y, const vec3 _in) {
  if (!init)
    return;
  int sample_x = (float)_x * (x - 1); //[0,x)
  int sample_y = (float)_y * (y - 1); //[0,y)
  int pos = sample_x * 3 + sample_y * x * 3;
#ifdef DEBUG
  std::cout << "Write in Buffer[" << pos << "]=" << _in << "\n";
#endif
  _buffer_map[pos] = static_cast<int>(_in.x());
  _buffer_map[pos + 1] = static_cast<int>(_in.y());
  _buffer_map[pos + 2] = static_cast<int>(_in.z());
}

inline void RGB12::writeBuffer(int _x, int _y, const vec3 _in) {
  if (!init)
    return;
  int pos = _x * 3 + _y * x * 3;
  _buffer_map[pos] = static_cast<int>(256 * clamp<double>(_in.x(), 0, 0.999));
  _buffer_map[pos + 1] =
      static_cast<int>(256 * clamp<double>(_in.y(), 0, 0.999));
  _buffer_map[pos + 2] =
      static_cast<int>(256 * clamp<double>(_in.z(), 0, 0.999));
}

inline void *RGB12::getData() const { return _buffer_map; }

// return in [0,1]
inline vec3 RGB12::sampler(double u, double v) const {
  if (!init) {
    return vec3(0, 0, 0);
  }
  float sample_x = (float)u * (x - 1);
  float sample_y = (float)v * (y - 1);
  // clamp sample x,y to [0,x);
  if (!linearInterpolation) {
    return sampler(static_cast<int>(sample_x), static_cast<int>(sample_y));
  } else {
    return BilinearSampler(sample_x, sample_y);
  }
}
inline vec3 RGB12::sampler(int u, int v) const {
  u = clamp(u, 0, x - 1);
  v = clamp(v, 0, v - 1);
  int pos = u * 3 + v * x * 3;
  return vec3(_buffer_map[pos], _buffer_map[pos + 1], _buffer_map[pos + 2]) *
         colorScale;
}

inline vec3 RGB12::BilinearSampler(float sample_x,float sample_y)const{
  int floor_x = floor(sample_x);
    int floor_y = floor(sample_y);
    int ceil_x = ceil(sample_x);
    int ceil_y = ceil(sample_y);
    float pxf = sample_x - floor_x;
    float pyf = sample_y - floor_y;
        return lerp(lerp(sampler(floor_x, floor_y), sampler(ceil_x, floor_y), pxf),
                lerp(sampler(floor_x, ceil_y), sampler(ceil_x, ceil_y), pxf),
                pyf);
}
#endif