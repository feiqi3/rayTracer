/*
 * @Author: feiqi3
 * @Date: 2022-05-12 11:10:47
 * @LastEditTime: 2022-05-15 12:16:07
 * @LastEditors: feiqi3
 * @Description: |---a buffer to save float3,or vec3 in glsl :)---|
 * @FilePath: \rayTracer\include\buffer\RGB12Buffer.h
 * ->blog: feiqi3.cn <-
 */

#ifndef RGB12BUUFFER_H
#define RGB12BUUFFER_H
                          
#include "../math/vector.h"
#include "Buffer.h"
#include "Macro.h"
#include <string>
#include <winuser.h>
class RGB12F : public buffer {
public:
  RGB12F(int _x, int _y) : buffer(_x, _y) {Flog::flog(INFO, toString());}
  ~RGB12F();

//Macro INFO
  GET_CLASS_NAME(RGB12);
  BUFFER_TO_STRING;


  bool make_buffer() override;
  void del_buffer() override;
  vec3 sampler(double x, double y) const override;
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

inline vec3 RGB12F::sampler(double _x, double _y) const {
  if (!init) {
    return vec3(0, 0, 0);
  }
  int sample_x =float(_x * (x - 1));
  int sample_y =float(_y * (y - 1));
  // clamp sample x,y to [0,x);
  sample_x = (sample_x > x - 1 ? x - 1 : sample_x) < 0 ? 0 : sample_x;
  sample_y = (sample_y > y - 1 ? y - 1 : sample_y) < 0 ? 0 : sample_y;
  int pos = sample_x * 3 + sample_y * x * 3;
#ifdef DEBUG
  std::cout << "Load from Buffer[" << pos << "]\n";
#endif
  return vec3(_buffer_map[pos], _buffer_map[pos + 1], _buffer_map[pos + 2]);
}

inline void RGB12F::writeBufferf(float _x, float _y, const vec3 _in) {
  int sample_x =(float) _x * (x - 1); //[0,x)
  int sample_y =(float) _y * (y - 1); //[0,y)
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
  RGB12(int _x, int _y) : buffer(_x, _y) {Flog::flog(INFO, toString());}
  ~RGB12();


  GET_CLASS_NAME(RGB12);
  BUFFER_TO_STRING;

  
  bool make_buffer() override;
  void del_buffer() override;
  vec3 sampler(double x, double y) const override;
  void writeBufferf(float x, float y, const vec3 in) override;
  void writeBuffer(int x, int y, const vec3 in) override;
  virtual void *getData() const override;
  
protected:
  // vec3 **bufferMap;
  unsigned char *_buffer_map;
};

inline RGB12::~RGB12() {
  if (init == true) {
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
  if (init == false) {
    return;
  }

  delete[] _buffer_map;
  init = false;
}

inline vec3 RGB12::sampler(double _x, double _y) const {
  if (!init) {
    return vec3(0, 0, 0);
  }
  int sample_x =(float) _x * (x - 1);
  int sample_y =(float) _y * (y - 1);
  // clamp sample x,y to [0,x);
  sample_x = (sample_x > x - 1 ? x - 1 : sample_x) < 0 ? 0 : sample_x;
  sample_y = (sample_y > y - 1 ? y - 1 : sample_y) < 0 ? 0 : sample_y;
  int pos = sample_x * 3 + sample_y * x * 3;
#ifdef DEBUG
  std::cout << "Load from Buffer[" << pos << "]\n";
#endif
  return vec3(_buffer_map[pos], _buffer_map[pos + 1], _buffer_map[pos + 2]);
}

inline void RGB12::writeBufferf(float _x, float _y, const vec3 _in) {
  int sample_x =(float) _x * (x - 1); //[0,x)
  int sample_y =(float) _y * (y - 1); //[0,y)
  int pos = sample_x * 3 + sample_y * x * 3;
#ifdef DEBUG
  std::cout << "Write in Buffer[" << pos << "]=" << _in << "\n";
#endif
  _buffer_map[pos] = static_cast<int>(_in.x());
  _buffer_map[pos + 1] = static_cast<int>(_in.y());
  _buffer_map[pos + 2] = static_cast<int>(_in.z());
}

inline void RGB12::writeBuffer(int _x, int _y, const vec3 _in) {
  int sample_x = _x;
  int sample_y = _y;

  int pos = sample_x * 3 + sample_y * x * 3;
#ifdef DEBUG
  std::cout << "Write in Buffer[" << pos << "]=" << _in << "\n";
#endif
  _buffer_map[pos] = static_cast<int>(256 * clamp<double>(_in.x(), 0, 0.999));
  _buffer_map[pos + 1] = static_cast<int>(256 * clamp<double>(_in.y(), 0, 0.999));
  _buffer_map[pos + 2] = static_cast<int>(256 * clamp<double>(_in.z(), 0, 0.999));
  if (_x == 3 && _y == 106)
   auto c=  _buffer_map[pos + 2];
}

inline void *RGB12::getData() const { return _buffer_map; }


#endif