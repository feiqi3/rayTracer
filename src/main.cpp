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
#include "noise/perlin_noise_3d.h"
#include "object/aarect.h"
#include "object/box.h"
#include "object/bvh_node.h"
#include "object/constant_medium.h"
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
constexpr int IMG_WIDTH = 1920;
constexpr double RATIO = 1.5;
constexpr int SAMPLES = 1000;
#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"
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
/*
int main() {
  srand(time(0));
  constexpr auto IMG_HEIGHT = static_cast<int>(IMG_WIDTH / RATIO);
  hitable_list world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto greenLit = make_shared<lambertian>((color(168, 247, 218) / 255));

  auto light = make_shared<diffuse_light>(color(1.3, 1.3, 1.3));
  auto lightRed = make_shared<diffuse_light>(color(1.3, .3, .3));
  auto lightBlue = make_shared<diffuse_light>(color(.3, .3, 1.2));
  auto lightGreen = make_shared<diffuse_light>(color(.3, 1.2, .3));
  auto glass = make_shared<dielectric>(1.5);

  auto earthTexture =
      make_shared<image_texture>("resource/texture/earthmap.jpg", true);
  auto earthMat = make_shared<lambertian>(earthTexture);
  perlin3D noise;
  auto lightAbove = make_shared<xz_rect>(-10, 10, 0, 25, 30, light);
  auto lightLeft = make_shared<yz_rect>(10, 35, 10, 25, -35, light);
  world.add(lightLeft);

  auto earth = make_shared<sphere>(vec3(-6, 17, 25), 5, earthMat);
    auto sphere2 = make_shared<sphere>(vec3(6, 10, 10), 4, white);
  auto fog = make_shared<constant_medium>(sphere2, 0.2, vec3(0.7,0.9,0.7));
  //world.add(fog);
  world.add(lightAbove);
  world.add(earth);
  vec3 cubePosMin(0, -5, 0), cubePosMax(8, 5, 8);
  for (int i = -3; i < 8; i++) {
    for (int j = -3; j < 8; j++) {
      vec3 tmpPos(-20 + 8 * i, 0, 50 - 8 * j);
      float height = noise.noise(tmpPos / 16);
      tmpPos[1] = height * 6;
      if ((i == 3 && (j == 0))) {
        tmpPos[1] = 10;
        auto mBox = make_shared<box>(cubePosMin + tmpPos, cubePosMax + tmpPos,
                                     lightRed);
        world.add(mBox);
        continue;
      }
      if ((i == 2 && (j == 5))) {
        tmpPos[1] = 4;
        auto mBox = make_shared<box>(cubePosMin + tmpPos, cubePosMax + tmpPos,
                                     lightBlue);
        world.add(mBox);
        continue;
      }
      if ((i == -1 && (j == 2))) {
        tmpPos[1] = 7;
        auto mBox = make_shared<box>(cubePosMin + tmpPos, cubePosMax + tmpPos,
                                     lightGreen);
        world.add(mBox);
        continue;
      }
      if ((i == 2 && (j == 2))) {
        tmpPos[1] = 4;
        auto mBox =
            make_shared<box>(cubePosMin + tmpPos, cubePosMax + tmpPos, light);
        world.add(mBox);
        continue;
      }
      auto mBox =
          make_shared<box>(cubePosMin + tmpPos, cubePosMax + tmpPos, greenLit);
      world.add(mBox);
    }
  }
  vec3 cameraPos(0, 20, 0);

  camera cam(55, RATIO, cameraPos, vec3(0, 1, 0), vec3(-5, 14, 25), 0,
             (vec3(0, 0, -1) - cameraPos).length());
  float start_time = clock();
  bvh_node bvh(world);

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
  std::cout << (end_time - start_time) / 60000 << "mins\n";
  fPic::jpgWriter(&mainBuffer);
  std::cout << "Done!";
}
*/  
int main(){
 auto m = mat::getTBN(normalize(vec3(1,1,1)));
 vec3 nn = normalize(vec3(1));
 for (int i = 0 ; i < 340 ; i++) {
    auto dir =sample_on_hemi(nn);
    assert(dot(dir,nn) > 0);
 }
}