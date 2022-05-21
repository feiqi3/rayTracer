/*
 * @Author: feiqi3
 * @Date: 2022-01-24 20:06:53
 * @LastEditTime: 2022-05-21 22:57:42
 * @LastEditors: feiqi3
 * @Description: |main application|
 * @FilePath: \rayTracer\src\main.cpp
 * ->blog: feiqi3.cn <-
 */
#include "../include/HeaderFiles.h"
#include "Macro.h"
#include "RenderQueue.h"
#include "buffer/RGB12Buffer.h"
#include "material/texture.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "object/texture_rectangle.h"
#include "object/texture_triangle.h"
#include "object/model.h"
#include "renderPass/renderPass.h"
#include "tool/fLoader.h"

#include <iostream>
#include <memory>

constexpr int IMG_WIDTH = 1920;
constexpr double RATIO = 16.0 / 9.0;
constexpr int SAMPLES = 40;

int main() {
  Flog logger();
  Flog::set_glob_log_level(TRACE);
  hitable_list world;
  shared_ptr<lambertian> ground_mat =
      std::make_shared<lambertian>(color(0.8, 0.8, 0.8));
  shared_ptr<metal> metal_sphere_a =
      std::make_shared<metal>(color(0.5, 0.5, 0.5), 0);
  shared_ptr<metal> metal_sphere_b =
      std::make_shared<metal>(color(0.1, 0.5, 0.3), 1);
  shared_ptr<lambertian> lambertian_sphere =
      std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
  shared_ptr<dielectric> die = make_shared<dielectric>(1.5);
  shared_ptr<dielectric> skm =
      std::make_shared<dielectric>(0.8);
  std::shared_ptr<RGB12> text_buffer = std::make_shared<RGB12>("./DSC01859.jpg");
  auto rectangle = make_shared<texture_rectangle>(vec3(-5, -2, -2), vec3(-5, 2, -2), vec3(5, 2, -2),
                         vec3(5, -2, -2),text_buffer,skm);
                         rectangle->init();
  mat4 trans = mat4::Identity();
  trans = mat::getRotate(1./4*3.14159,vec3(0,1,0));
  rectangle->transform(trans);
  auto shperea = make_shared<sphere>(vec3(0, 0, -1), .5, lambertian_sphere);
  auto shpereb = make_shared<sphere>(vec3(-1.0, 0, -1), .5, metal_sphere_a);
  auto spherec = make_shared<sphere>(vec3(1.0, 0, -1), .5, die);
  auto sphered = make_shared<sphere>(vec3(0, -100.5, -2), 99, ground_mat);
  
/*   world.add(make_shared<sphere>(vec3(0, 0, -1), .5, lambertian_sphere));
  world.add(make_shared<sphere>(vec3(-1.0, 0, -1), .5, metal_sphere_a));
  world.add(rectangle);

  world.add(make_shared<sphere>(vec3(1.0, 0, -1), .5, die));

  world.add(make_shared<sphere>(vec3(0, -100.5, -2), 99, ground_mat)); */

  int img_width = IMG_WIDTH;
  int img_height = static_cast<int>(img_width / RATIO);
  //  ppmMaker pm(img_width, img_height);

  vec3 cameraPos(3, 3, 2);
#ifdef DEBUG
  std::cout << "Focus length  " << (vec3(0, 0, -1) - cameraPos).length()
            << "\n";
#endif
shared_ptr<renderPass> cam = make_shared<camera>(30, RATIO, vec3(-2, -2, 5), vec3(0, 1, 0), vec3(0, 0, -1));
renderQueue rq(cam,IMG_WIDTH,16./9,true);
floader f;
//f.fload("./model/lowpolytree.obj");
//rq.addObj(f.getModel());
rq.addObj(shperea);
rq.addObj(shpereb);
rq.addObj(spherec);
rq.addObj(sphered);
rq.addObj(rectangle);
rq.setThreadNum(9);
rq.MultiThreadRender();
rq.SaveToFile();
/*   double division_x = 1.0 / (img_width - 1.0);
  double division_y = 1.0 / (img_height - 1.0);

  constexpr auto divided_samples = 1.0 / SAMPLES;

  constexpr float theta = 256 * divided_samples;

  int max_dep = 100;

  buffer *main_fram = new RGB12(img_width, img_height);
  main_fram->make_buffer();
  // render from top left to bottom right
  for (int y = img_height - 1; y >= 0; --y) {
    for (int x = 0; x < img_width; ++x) {
      color pxl(0, 0, 0);
      for (std::size_t sample_times = 0; sample_times < SAMPLES;
           sample_times++) {

        // transform to NDC(?)
        auto u = (x + rand_d()) * division_x;
        auto v = (y + rand_d()) * division_y;
        ray tmp_ray = cam.get_ray(u, v);
        pxl += cam.cast_ray(tmp_ray, world, max_dep);
      }
      main_fram->writeBuffer(x, y, (pxl * divided_samples));
      // pm.colorWrite(main_fram->sampler(x * division_x, y * division_y));
      //      pm.m_s_colorWirte(pxl,divided_samples);
    }
#ifndef DEBUG
     if (y % ((int)(img_height / 100)) == 0) {
      std::cout << "render :"
                << (int)((1.0 - (double)y / (img_height - 1.0)) * 100) << "\n";
    }
#endif
  }
  fPic::jpgWriter(main_fram);
  Flog::flog(INFO, "Render finished!");
  delete main_fram; */
}
