/*
 * @Author: feiqi3
 * @Date: 2022-05-14 13:27:31
 * @LastEditTime: 2022-05-21 13:03:14
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\tool\picTools.h
 * ->blog: feiqi3.cn <-
 */
#ifndef PICTOOLS_H
#define PICTOOLS_H
#include "buffer/RGB12Buffer.h"
#include <iostream>
#include <stdio.h>
#include <system_error>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "Vendor/stb_image.h"
#include "Vendor/stb_image_write.h"
#include <string.h>
#include <string>

namespace fPic {
struct img_info {
  int x;
  int y;
  int channel;
};


//RGB12 Buffer only
inline void jpgWriter(RGB12 *_buffer, const char *path, const char *name,
                      int flip_vertically) {
  if (flip_vertically == 1) {
    stbi_flip_vertically_on_write(1);
  }
  std::string destination = path;
  destination += name;
  stbi_write_jpg(destination.c_str(), _buffer->buf_width(), _buffer->buf_height(), 3,
                 (int *)_buffer->getData(), 100);
  stbi_flip_vertically_on_write(0);
}

//RGB12 Buffer only
inline void jpgWriter(RGB12 *_buffer) {
  jpgWriter(_buffer, "./", "out.jpg", 1);
}

//load rgb from picture
inline unsigned char *load_from_jpg(const char *filename, img_info &_info) {
  int x, y, n;
  stbi_set_flip_vertically_on_load(1);
  unsigned char * output = stbi_load(filename, &x, &y, &n, 3);
  if(output == nullptr)
      std::cout<<"ERROR occurred when load picture";
  _info.x = x;
  _info.y = y;
  _info.channel = n;
  stbi_set_flip_vertically_on_load(0);
  return output;
}

inline void picDelete(unsigned char*p){
    STBI_FREE(p);
}

} // namespace fPic

#endif