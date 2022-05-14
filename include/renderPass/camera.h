/*
 * @Author: feiqi3
 * @Date: 2022-02-08 10:43:24
 * @LastEditTime: 2022-05-13 16:55:46
 * @LastEditors: feiqi3
 * @Description: |camera class|
 * @FilePath: \rayTracer\include\renderPass\camera.h
 * ->blog: feiqi3.cn <-
 */

#ifndef _CAMERA_H
#define _CAMERA_H

#include "../material/material.h"
#include "../math/vector.h"
#include "../ray.h"
#include "renderPass.h"
#include "../tool/common.h"
#include <cmath>
#include <iostream>
#include <stdio.h>

class camera : public renderPass {
private:
  vec3 lower_left_corner;
  vec3 horizontal, vertical;
  float lens_radius;

public:
  camera(float _fov, float _ratio, vec3 _pos, vec3 _up, vec3 lookat,
         float _aperture = 0, float focus_dist = 1)
      : renderPass(_fov, _ratio, _pos, _up, lookat), focal_length(focus_dist),
        focal_viewport_height(renderPass::viewport_height * focal_length),
        focal_viewport_width(renderPass::ratio * focal_viewport_height),
        aperture(_aperture) {
    horizontal = focal_viewport_width *renderPass::u;
    vertical = focal_viewport_height *renderPass::v;
    lens_radius = aperture / 2;
    lower_left_corner =
        renderPass::origin - horizontal / 2 - vertical / 2 - renderPass::w * focal_length;

    // Note here : The order of cross product does affect result
#ifdef DEBUG
    std::cout << "U" << u << "\n";
    std::cout << "V" << v << "\n";
    std::cout << "W" << w << "\n";

    std::cout << "Fov vertical " << fov_vertical << "\n";

    std::cout << "Viewport height " << viewport_height << "\n";

    std::cout << "Horizontal" << horizontal << "\n";
    std::cout << "Vertical" << vertical << "\n";
    std::cout << "Look dir" << lookDir << "\n";
    std::cout << "Lower left corner" << lower_left_coner << "\n";

#endif
  }

  ray get_ray(double s, double t) {
    vec3 rand_p = get_rand_in_disk() * lens_radius;
    vec3 offset = u * rand_p.x() + v * rand_p.y();
    return ray(origin + offset, lower_left_corner + s * horizontal +
                                    t * vertical - (origin + offset));
  }

color cast_ray(const ray &r, const hitable_list &world, int depth) {
  record rec;
  depth = depth - 1;
  if (depth <= 0) {
    return color(0, 0, 0);
  }
  if (world.hit(r, 0.001, Infinity, rec)) {
    color attenuation;
    ray scattered;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * cast_ray(scattered, world, depth);
    }
    return color(0, 0, 0);
  }
  vec3 unit_dir =r.direction();//in ray.class it has been normalized
  double t = 0.5 * (unit_dir.y() + 1);
  // a linear interpolation
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

public:
  const float focal_length;
  const float focal_viewport_height;
  const float focal_viewport_width;
  const float aperture;
};

#endif
