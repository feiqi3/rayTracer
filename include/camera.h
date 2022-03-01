/*
 * @Author: feiqi3
 * @Date: 2022-02-08 10:43:24
 * @LastEditTime: 2022-02-28 22:23:19
 * @LastEditors: feiqi3
 * @Description: |camera class|
 * @FilePath: \rayTracer\include\camera.h
 * ->blog: feiqi3.cn <-
 */

#ifndef _CAMERA_H
#define _CAMERA_H

#include "math/vector.h"
#include "ray.h"
#include "tool/common.h"
#include <iostream>

class camera {
private:
  vec3 lower_left_coner;
  vec3 horizontal, vertical;

public:
  camera(float viewport_width_, float viewport_height_, float ratio_)
      : origin(0, 0, 0), viewport_width(viewport_width_), ratio(ratio_),
        viewport_height(viewport_height_), focal_length(1.0) {
    horizontal = vec3(viewport_width, 0, 0);
    vertical = vec3(0, viewport_height, 0);
    lower_left_coner =
        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    //std::cout << lower_left_coner << "\n";
  }

  ray get_ray(double u, double v) {
    return ray(origin, lower_left_coner + u * horizontal + v * vertical);
  }

public:
  const vec3 origin;
  const float ratio;
  const float focal_length;
  const float viewport_height;
  const float viewport_width;
};

#endif
