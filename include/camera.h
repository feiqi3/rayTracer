/*
 * @Author: feiqi3
 * @Date: 2022-02-08 10:43:24
 * @LastEditTime: 2022-03-28 16:40:15
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
#include <cmath>
#include <iostream>
#include <stdio.h>

class camera {
private:
  vec3 lower_left_coner;
  vec3 horizontal, vertical;
public:
  camera(float _fov, float _ratio,vec3 pos,vec3 look_at,vec3 up)
      : origin(pos), ratio(_ratio), focal_length(1), fov_vertical(_fov),
        viewport_height(2 * focal_length * tan(d2radian(fov_vertical / 2))),
        viewport_width(ratio * viewport_height)
        {
    vec3 cam_up = up;
    vec3 cam_pos = origin;
    vertical =cam_up;
    vec3 lookDir = normalize(cam_pos - look_at);
    //Note here : The order of cross product does affect result
    horizontal =normalize(cross(vertical,lookDir));
    vertical = cross(lookDir,horizontal);
    horizontal = horizontal * viewport_width;
    vertical = vertical * viewport_height;
#ifdef DEBUG
    std::cout<<"Horizontal"<<horizontal<<"\n";
    std::cout<<"Vertical"<<vertical<<"\n";
    std::cout<<lookDir<<"\n";\
    std::cout <<"Lower left corner"<<lower_left_coner << "\n";

#endif
    lower_left_coner = origin - horizontal / 2 - vertical / 2 - lookDir;
  }

  ray get_ray(double u, double v) {
    return ray(origin, lower_left_coner + u * horizontal + v * vertical - origin);
  }

public:
  const vec3 origin;
  const float fov_vertical;
  const float ratio;
  const float focal_length;
  mutable float viewport_height;
  mutable float viewport_width;
};

#endif
