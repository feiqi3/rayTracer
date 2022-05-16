/*
 * @Author: feiqi3
 * @Date: 2022-05-12 10:34:58
 * @LastEditTime: 2022-05-15 13:35:34
 * @LastEditors: feiqi3
 * @Description: |---
Buffer base class
Buffer is an object that can save DATA!
it also can be written to PIC!(coming soon
I supposed to have three buffers
R4,->float
RGB12->float vec3
RGB24->double vec3
 ---|
 * @FilePath: \rayTracer\include\buffer\Buffer.h
 * ->blog: feiqi3.cn <-
 */
#ifndef BUFFER_H
#define BUFFER_H
#include "../math/vector.h"
#include "../tool/flog.h"
#include <string>


#define BUFFER_INFO_STR                                                        \
  className + ",buffer width=" + STR(buffer::x) +                  \
      ",buffer height=" + STR(buffer::y)+".\n"

#define BUFFER_TO_STRING                                                       \
  virtual const std::string toString() const override{                                     \
    std::string className = clsname();                                         \
    return BUFFER_INFO_STR;                                                    \
  }
  

class buffer {
public:
  buffer(int x, int y);
  virtual ~buffer();
  virtual bool is_init() const;
  // malloc mem for buffer
  virtual bool make_buffer() = 0;
  // delete buffer
  virtual void del_buffer() = 0;
  // sample buffer
  virtual vec3 sampler(double x, double y) const = 0;
  // write to buffer
  virtual void writeBufferf(float x, float y, const vec3 _in) = 0;
  virtual void writeBuffer(int x, int y, const vec3 _in) = 0;
  int buf_width() const;
  int buf_height() const;
  virtual void *getData() const = 0;
  virtual const std::string toString() const = 0;

protected:
  // buffer status
  bool init;
  // buffer size
  int x, y;
};

// width and height
inline buffer::buffer(int _x, int _y) : x(_x), y(_y) { init = false; }
inline buffer::~buffer() {}
inline bool buffer::is_init() const { return init; }
inline int buffer::buf_width() const { return x; }
inline int buffer::buf_height() const { return y; }

#endif