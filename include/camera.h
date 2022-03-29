/*
 * @Author: feiqi3
 * @Date: 2022-02-08 10:43:24
 * @LastEditTime: 2022-03-29 14:23:59
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
  vec3 w, u, v;
  float lens_radius;
public:
  camera(float _fov, float _ratio, vec3 pos, vec3 up, vec3 look_at,
         float _aperture,float focus_dist)
      : origin(pos), fov_vertical(_fov), ratio(_ratio),
        focal_length(focus_dist),
        viewport_height(2 * tan(d2radian(fov_vertical / 2.0)) * focal_length),
        viewport_width(ratio * viewport_height),aperture(_aperture) {
    vec3 lookDir =  pos - look_at;
    w = normalize(lookDir);
    u =normalize(cross(up, w));
    v = cross(w, u);
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lens_radius = aperture / 2;
    lower_left_coner = origin - horizontal / 2 - vertical / 2 - w * focal_length;
    
    // Note here : The order of cross product does affect result
#ifdef DEBUG
    std::cout << "U" << u << "\n";
    std::cout << "V" << v << "\n";
    std::cout << "W" << w << "\n";

    std::cout<<"Fov vertical "<<fov_vertical<<"\n";

    std::cout << "Viewport height " << viewport_height << "\n";


    std::cout << "Horizontal" << horizontal << "\n";
    std::cout << "Vertical" << vertical << "\n";
    std::cout << "Look dir" <<lookDir << "\n";
    std::cout << "Lower left corner" << lower_left_coner << "\n";

#endif
  }

  ray get_ray(double s, double t) {
    vec3 rand_p = get_rand_in_disk() * lens_radius;
    vec3 offset = u * rand_p.x() + v * rand_p.y();
    return ray(origin + offset,
               lower_left_coner + s * horizontal + t * vertical - (origin + offset));
  }

public:
  const vec3 origin;
  const float fov_vertical;
  const float ratio;
  const float focal_length;
  const float viewport_height;
  const float viewport_width;
  const float aperture;
};

#endif
