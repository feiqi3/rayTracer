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
#include <string>
class RGB12F : public buffer {
public:
  RGB12F(int _x, int _y) : buffer(_x, _y) { make_buffer(); }
  ~RGB12F();
  bool make_buffer() override;
  void del_buffer() override;
  //vec3 sampler(double x, double y) const override;
  void writeBufferf(float x, float y, const vec3 in) override;
  void writeBuffer(int x, int y, const vec3 in) override;
  virtual void *getData() const override;

protected:
  // vec3 **bufferMap;
  float *_buffer_map;
};

inline RGB12F::~RGB12F() {
  if (init == true) {
    del_buffer();
  }
}

inline bool RGB12F::make_buffer() {
  del_buffer();
  _buffer_map = new float[x * y * 3];
  init = true;
  return true;
}

inline void RGB12F::del_buffer() {
  if (init == false) {
    return;
  }

  delete[] _buffer_map;
  init = false;
}

inline void RGB12F::writeBufferf(float _x, float _y, const vec3 _in) {
  int sample_x = (float)_x * (x - 1); //[0,x)
  int sample_y = (float)_y * (y - 1); //[0,y)
  int pos = sample_x * 3 + sample_y * y * 3;
#ifdef DEBUG
  std::cout << "Write in Buffer[" << pos << "]=" << _in << "\n";
#endif
  _buffer_map[pos] = _in.x();
  _buffer_map[pos + 1] = _in.y();
  _buffer_map[pos + 2] = _in.z();
}

inline void RGB12F::writeBuffer(int _x, int _y, const vec3 _in) {
  int sample_x = _x;
  int sample_y = _y;
  int pos = sample_x * 3 + sample_y * x * 3;
#ifdef DEBUG
  std::cout << "Write in Buffer[" << pos << "]=" << _in << "\n";
#endif
  _buffer_map[pos] = _in.x();
  _buffer_map[pos + 1] = _in.y();
  _buffer_map[pos + 2] = _in.z();
}

inline void *RGB12F::getData() const { return _buffer_map; }

class RGB12 : public buffer {
public:
  RGB12(int _x, int _y) : buffer(_x, _y) { make_buffer(); }
  ~RGB12();
  bool make_buffer() override;
  void del_buffer() override;
  void writeBufferf(float x, float y, const vec3 in) override;
  void writeBuffer(int x, int y, const vec3 in) override;
  virtual void *getData() const override;

protected:
  // vec3 **bufferMap;
  unsigned char *_buffer_map;

private:
  bool load_from_file;
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

#endif