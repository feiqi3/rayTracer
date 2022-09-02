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
#include "material/diffuse_light.h"
#include "material/normal_shader.h"
#include "noise/perlin_noise_3d.h"
#include "object/aarect.h"
#include "object/box.h"
#include "object/bvh_node.h"
#include "object/hitable.h"
#include "object/sphere.h"
#include "object/triangle.h"
#include "texture/checker_texture.h"
#include "texture/image_texture.h"
#include "texture/marble_texture.h"
#include "texture/noise_texture.h"
#include "texture/turbulence_texture.h"
#include "tool/picTool.h"
#include <memory>
#include <stdlib.h>
#include <time.h>
constexpr int IMG_WIDTH = 300;
constexpr double RATIO = 1.0;
constexpr int SAMPLES = 100;
#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"
#include "math/vector.h"
#include "ray.h"
#include "tool/common.h"
#include "tool/ppmUtil.h"
#include <iostream>

const vec3 Back_Ground_Color(.7, .7, .7);

color rayTrace(const ray &r, const vec3 &background, hitable *world,
               int depth) {
  record rec;
  depth = depth - 1;
  if (depth <= 0) {
    return color(0, 0, 0);
  }
  if (!world->hit(r, 0.001, Infinity, rec))
    return background;
  ray scattered;
  vec3 attenuaion;
  vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
  // Only when ray hits a LIGHT, The function scatter will return false
  if (!rec.mat_ptr->scatter(r, rec, attenuaion, scattered)) {
    return emitted;
  }
  return emitted +
         attenuaion * rayTrace(scattered, background, world, depth - 1);
}

int main() {
  srand(time(0));
  constexpr auto IMG_HEIGHT = static_cast<int>(IMG_WIDTH / RATIO);
  hitable_list world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<diffuse_light>(color(15, 15, 15));
  auto glass = make_shared<dielectric>(1.5);
  world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
  world.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
  world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
  world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

  // world.add(make_shared<sphere>(vec3(278,100,200.5),100,glass));

  world.add(make_shared<box>(vec3(130, 0, 65), vec3(295, 165, 230), white));
  world.add(make_shared<box>(vec3(265, 0, 295), vec3(430, 330, 460), white));
  /*  world.add(
        make_shared<rectangle>(vec3(0, 555, 0), vec3(555, 555, 555), white));

    world.add(make_shared<rectangle>(vec3(0, 0, 0), vec3(555, 0, 555), white));
    world.add(
        make_shared<rectangle>(vec3(0, 0, 555), vec3(555, 555, 555), white));
      world.add(make_shared<rectangle>(vec3(555, 0, 0), vec3(555), green));
    world.add(make_shared<rectangle>(vec3(0, 0, 0), vec3(0, 555, 555), red));
    */
  float start_time = clock();

  bvh_node bvh(world);
  vec3 cameraPos(278, 278, -800);

  camera cam(40, RATIO, cameraPos, vec3(0, 1, 0), vec3(278, 278, 0), 0,
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
        pxl += rayTrace(tmp_ray, Back_Ground_Color, &world, max_dep);
      }
      pxl = pxl * divided_samples;
      mainBuffer.writeBuffer(x, y, pxl);
    }
  }
  float end_time = clock();
  std::cout << (end_time - start_time)<<"\n";
  fPic::jpgWriter(&mainBuffer);
  std::cout << "Done!";
}
