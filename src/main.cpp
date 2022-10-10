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
#include "math/matrix.h"
#include "Integrator/integrator.h"
#include "object/box.h"
#include "object/bvh_node.h"
#include "object/hitable.h"
#include "object/rotate_y.h"
#include "object/sphere.h"
#include "object/translate.h"
#include "object/triangle.h"
#include "texture/checker_texture.h"
#include "texture/image_texture.h"
#include "tool/picTool.h"
#include <cstdlib>
#include <memory>
#include <stdlib.h>
#include <time.h>
constexpr int IMG_WIDTH = 1000;
constexpr double RATIO = 1.5;
constexpr int SAMPLES = 300;
#include "material/lambertian.h"
#include "math/vector.h"
#include "ray.h"
#include "tool/common.h"
#include "tool/ppmUtil.h"
#include <iostream>

const vec3 Back_Ground_Color(.0, .0, .0);

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
  auto greenLit = make_shared<lambertian>((color(168, 247, 218) / 255));

  auto light = make_shared<diffuse_light>(color(1.3, 1.3, 1.3),white);
  auto earthTexture =
      make_shared<image_texture>("resource/texture/earthmap.jpg", true);
  auto earthMat = make_shared<lambertian>(earthTexture);

  auto earth = make_shared<sphere>(vec3(-6, 17, 25), 5, earthMat);
  auto plane_below = make_shared<sphere>(vec3(-6, -1000, 25),1006,white);
  world.add(earth);
  world.add(plane_below);
  auto lightAbove = std::make_shared<sphere>(vec3(-6, 35, 25),10,light);
  world.add(lightAbove);
  vec3 cameraPos(0, 20, 0);

  camera cam(55, RATIO, cameraPos, vec3(0, 1, 0), vec3(-5, 14, 25), 0,
             (vec3(0, 0, -1) - cameraPos).length());

  bvh_node bvh(world);

  baseIntegrator renderer(&bvh,IMG_HEIGHT,IMG_WIDTH,20,&cam,vec3(0,0.1,0.1));
  renderer.Render();
}
