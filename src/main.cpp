/*
 * @Author: feiqi3
 * @Date: 2022-01-24 20:06:53
 * @LastEditTime: 2022-03-01 23:22:44
 * @LastEditors: feiqi3
 * @Description: |main appliancation|
 * @FilePath: \rayTracer\src\main.cpp
 * ->blog: feiqi3.cn <-
 */

#include "camera.h"
#include "hitableList.h"
#include "object/hitable.h"
#include "object/sphere.h"
#include <memory>
constexpr int IMG_WIDTH = 1920;
constexpr double RATIO = 16.0 / 9.0;
constexpr int SAMPLES = 5;
#include "math/vector.h"
#include "ray.h"
#include "tool/common.h"
#include "tool/ppmUtil.h"
#include <iostream>

double sphere_disrimination(const vec3 &point, const float radius,
                            const ray &r) {
  vec3 o2o = r.orig - point;
  double a = dot(r.direction(), r.direction());
  double b = 2 * dot(r.direction(), (o2o));
  double c = dot(o2o, o2o) - radius * radius;
  double dis = b * b - 4 * a * c;
  if (dis > 0.0) {
    return (-b - sqrt(dis)) / (2 * a);
  }
  return -1.0;
}

color ray_test(const ray &r, const hitable_list &world,int depth) {
  record rec;
  depth = depth - 1;
  if (depth <= 0) {
   return color(0,0,0);
  }
  if (world.hit(r, 0.001, Infinity, rec)) {
    vec3 tar_dir = rec.p + rec.normal + rand_in_unit_sphere();
    return 0.5 * (ray_test(ray(rec.p,tar_dir-rec.p), world,depth));
  }
  vec3 unit_dir = normalize(r.direction());
  double t = 0.5 * (unit_dir.y() + 1);
  // a linear interpolation
  return (1.0 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1.0);
}

int main() {

  hitable_list world;
  world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

  int img_width = IMG_WIDTH;
  int img_height = static_cast<int>(img_width / RATIO);
  ppmMaker pm(img_width, img_height);

  camera cam(2.0 * RATIO, 2.0, RATIO);

  double division_x = 1.0 / (img_width - 1);
  double division_y = 1.0 / (img_height - 1);

  auto divided_samples = 1.0 / SAMPLES;

  int max_dep = 100;

  // render from top left to bottom right
  for (int y = img_height - 1; y >= 0; --y) {
    for (int x = 0; x < img_width; ++x) {
      color pxl(0, 0, 0);
      for (std::size_t sample_times = 0; sample_times < SAMPLES;
           sample_times++) {
        auto u = (x + rand_d()) * division_x;
        auto v = (y + rand_d()) * division_y;
        ray tmp_ray = cam.get_ray(u, v);
        pxl += ray_test(tmp_ray, world,max_dep);
      }
      pm.m_s_colorWirte(pxl, divided_samples);
    }
    if ((int)(y / 10))
      std::cout << "render :"
                << (int)((1.0 - (double)y / (img_height - 1)) * 100) << "\n";
  }
  std::cout << "Done!";
}
