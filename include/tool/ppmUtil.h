/*
 * @Author: feiqi3
 * @Date: 2022-01-24 20:08:20
 * @LastEditTime: 2022-03-04 10:33:13
 * @LastEditors: feiqi3
 * @Description: |make ppm file|
 * @FilePath: \rayTracer\include\tool\ppmUtil.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _PPMUTIL_H_
#define _PPMUTIL_H_

#include "math/vector.h"
#include "tool/common.h"
#include <exception>
#include <fstream>
#include <string>

class ppmMaker {
private:
  std::ofstream outBuffer;

public:
  ppmMaker(int img_width, int img_height) {
    outBuffer = std::ofstream("out.ppm");
    if (!outBuffer) {
      std::cout << "Failed when open File!";
      abort;
    } else {
      std::cout << "open success\n";
      outBuffer << "P3\n"
                << img_width << " " << img_height << "\n"
                << 255 << "\n";
    }
  }

  ~ppmMaker() { outBuffer.close(); }

  color gamma_correction(const color &in)
  {
    return color(InvSqrt(in.x()),InvSqrt(in.y()),InvSqrt(in.z()));
  }

  void colorWrite(const color &in_color) {
    color correction = gamma_correction(in_color);
    outBuffer << static_cast<int>(256 * clamp<double>( correction.x(), 0, 0.999))
              << " "
              << static_cast<int>(256 * clamp<double>( correction.y(), 0, 0.999))
              << " "
              << static_cast<int>(256 * clamp<double>( correction.z(), 0, 0.999))
              << "\n";
  }

 //@param: color, 1 / sample times
  void m_s_colorWirte(const color &_color, const double samples_num_divided) {
    color tmp(_color);
    tmp =tmp * samples_num_divided;
    colorWrite(tmp);
  }
};

#endif