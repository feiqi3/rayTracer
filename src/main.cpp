/*
 * @Author: feiqi3
 * @Date: 2022-01-24 20:06:53
 * @LastEditTime: 2022-03-29 14:37:34
 * @LastEditors: feiqi3
 * @Description: |main application|
 * @FilePath: \rayTracer\src\main.cpp
 * ->blog: feiqi3.cn <-
 */
#include "Macro.h"
#include "buffer/RGB12Buffer.h"
#include "camera.h"
#include "hitableList.h"
#include "material/normal_shader.h"
#include "noise/perlin_noise_3d.h"
#include "object/bvh_node.h"
#include "object/hitable.h"
#include "object/sphere.h"
#include "texture/checker_texture.h"
#include "texture/image_texture.h"
#include "texture/marble_texture.h"
#include "texture/noise_texture.h"
#include "texture/turbulence_texture.h"
#include "tool/picTool.h"
#include <memory>
#include <stdlib.h>
constexpr int IMG_WIDTH = 1000;
constexpr double RATIO = 16.0 / 9.0;
constexpr int SAMPLES = 100;
#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"
#include "math/vector.h"
#include "ray.h"
#include "tool/common.h"
#include "tool/ppmUtil.h"
#include <iostream>

color rayTrace(const ray &r, hitable *world, int depth) {
  record rec;
  depth = depth - 1;
  if (depth <= 0) {
    return color(0, 0, 0);
  }
  if (world->hit(r, 0.001, Infinity, rec)) {
    color attenuation;
    ray scattered;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * rayTrace(scattered, world, depth);
    }
    return color(0, 0, 0);
  }
  vec3 unit_dir = normalize(r.direction());
  double t = 0.5 * (unit_dir.y() + 1);
  // a linear interpolation
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  srand(time(0));
  constexpr auto IMG_HEIGHT = static_cast<int>(IMG_WIDTH / RATIO);
  auto earthTexture = make_shared<image_texture>("resource/texture/earthmap.jpg");



  perlin3D n;
  auto colorTexA = std::make_shared<constant_color>(vec3(1, 1, 1));
  auto colorTexB = std::make_shared<constant_color>(vec3(.1, .7, .7));
  shared_ptr<metal> metal_sphere_a =
      std::make_shared<metal>(color(0.5, 0.5, 0.5), 0);
  shared_ptr<metal> metal_sphere_b =
      std::make_shared<metal>(color(0.1, 0.5, 0.3), 1);
  shared_ptr<lambertian> lambertian_sphere =
      std::make_shared<lambertian>(vec3(0.2, 0.1, 0.6));
  shared_ptr<dielectric> die = make_shared<dielectric>(1.5);
  auto checker = make_shared<checker_texture>(colorTexA, colorTexB);
  shared_ptr<noise_texture> turb_noise_tex = make_shared<noise_texture>(5.5);
  hitable_list world;
  shared_ptr<lambertian> ground_mat = std::make_shared<lambertian>(checker);
  auto turb_mat = std::make_shared<lambertian>(turb_noise_tex);
  auto earth_mat = std::make_shared<lambertian>(earthTexture);
  world.add(make_shared<sphere>(vec3(0, 0, -1), .5, turb_mat));
  world.add(make_shared<sphere>(vec3(-1.0, 0, -1), .5, metal_sphere_a));
  world.add(make_shared<sphere>(vec3(1.0, 0, -1), 0.5, die));
  world.add(make_shared<sphere>(vec3(0, -30.5, -1), 30, earth_mat));
  bvh_node bvh(world);
  vec3 cameraPos(3, 3, 20);

  camera cam(30, RATIO, cameraPos, vec3(0, 1, 0), vec3(0, 0, -1), 0,
             (vec3(0, 0, -1) - cameraPos).length());
  RGB12 mainBuffer(IMG_WIDTH, IMG_HEIGHT);

  double division_x = 1.0 / (IMG_WIDTH - 1.0);
  double division_y = 1.0 / (IMG_HEIGHT - 1.0);

  auto divided_samples = 1.0 / SAMPLES;

  int max_dep = 100;

  // render from top left to bottom right
  for (int y = IMG_HEIGHT - 1; y >= 0; --y) {
    for (int x = 0; x < IMG_WIDTH; ++x) {
      color pxl(0, 0, 0);
      for (std::size_t sample_times = 0; sample_times < SAMPLES;
           sample_times++) {

        // transform to NDC(?)
        auto u = (x + rand_d()) * division_x;
        auto v = (y + rand_d()) * division_y;
        ray tmp_ray = cam.get_ray(u, v);
        pxl += rayTrace(tmp_ray, &world, max_dep);
      }
      pxl = pxl * divided_samples;
      mainBuffer.writeBuffer(x, y, pxl);
    }
  }
  fPic::jpgWriter(&mainBuffer);
  std::cout << "Done!";
}
