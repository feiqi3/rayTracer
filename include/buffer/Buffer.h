/*
 * @Author: feiqi3
 * @Date: 2022-05-12 10:34:58
 * @LastEditTime: 2022-05-12 12:56:59
 * @LastEditors: feiqi3
 * @Description: |---
Buffer base class 
Buffer is an object that can save DATA!
it also can be rendered to PIC!(coming soon
I supposed to have two buffer
R16,
RGB16
 ---|
 * @FilePath: \rayTracer\include\buffer\Buffer.h
 * ->blog: feiqi3.cn <-
 */
#ifndef BUFFER_H
#define BUFFER_H

#include "../math/vector.h"
class buffer {
public:
  buffer(int x, int y);
  virtual ~buffer();
  virtual bool is_init() const;
  //malloc mem for buffer 
  virtual bool make_buffer() = 0;
  //delete buffer 
  virtual void del_buffer() = 0;
  //sample buffer
  virtual vec3 sampler(float x, float y) const = 0;
  //write to buffer
  virtual void writeBuffer(float x, float y, const vec3 _in) = 0;

protected:
  //buffer status
  bool init;
  //buffer size
  int x, y;
};
inline buffer::buffer(int _x, int _y) : x(_x), y(_y) {}
inline buffer::~buffer() {}
inline bool buffer::make_buffer() {}
inline vec3 buffer::sampler(float x, float y) const {}
inline void buffer::del_buffer() {}
inline void buffer::writeBuffer(float x, float y, const vec3 _in) {}
inline bool buffer::is_init() const { return init; }
#endif