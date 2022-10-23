/*
 * @Author: feiqi3
 * @Date: 2022-01-24 20:06:53
 * @LastEditTime: 2022-03-29 14:37:34
 * @LastEditors: feiqi3
 * @Description: |main application|
 * @FilePath: \rayTracer\src\main.cpp
 * ->blog: feiqi3.cn <-
 */
#include "Integrator/integrator.h"
#include "Macro.h"
#include "buffer/RGB12Buffer.h"
#include "camera.h"
#include "hitableList.h"
#include "material/diffuse_light.h"
#include "material/microfacet.h"
#include "math/matrix.h"
#include "object/box.h"
#include "object/bvh_node.h"
#include "object/hitable.h"
#include "object/rotate_y.h"
#include "object/sphere.h"
#include "object/translate.h"
#include "object/triangle.h"
#include "texture/checker_texture.h"
#include "texture/constant_color.h"
#include "texture/constant_roughness.h"
#include "texture/image_texture.h"
#include "tool/picTool.h"
#include <cstdlib>
#include <memory>
#include <stdlib.h>
#include <time.h>

constexpr int IMG_WIDTH = 1920;
constexpr double RATIO = 1.5;
constexpr int SAMPLES = 300;
#include "material/lambertian.h"
#include "math/vector.h"
#include "ray.h"
#include "tool/common.h"
#include "tool/ppmUtil.h"
#include <iostream>

const vec3 Back_Ground_Color(.0, .0, .0);


int main() {
  constexpr auto IMG_HEIGHT = static_cast<int>(IMG_WIDTH / RATIO);
  hitable_list world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto greenLit = make_shared<lambertian>((color(168, 247, 218) / 255));
  auto light = make_shared<diffuse_light>(color(1, 1, 1),white);
  auto box_an = make_shared<microfacet>(make_shared<constant_color>(vec3(.77)),make_shared<cRough>(0.2),vec3(0.59));
  auto box_in = make_shared<microfacet>(make_shared<constant_color>(.73),make_shared<cRough>(0.01,0.005),vec3(0.6));



  world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
  world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
 // world.add(make_shared<xz_rect>(200, 343+13, 227-13, 332+13, 550, light));
  world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
  world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, light));
  world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
  // world.add(make_shared<sphere>(vec3(278,100,200.5),100,glass));
  shared_ptr<hitable> box1 =
      make_shared<box>(vec3(0, 0, 0), vec3(165, 330, 165), box_in);
  box1 = make_shared<rotate_y>(box1, 15);
  box1 = make_shared<translate>(box1, vec3(265, 0, 295));
  shared_ptr<hitable> box2 =
      make_shared<box>(vec3(0, 0, 0), vec3(165, 165, 165), box_an);
  box2 = make_shared<rotate_y>(box2, -18);
  box2 = make_shared<translate>(box2, vec3(130, 0, 65));
  world.add(box1);
  world.add(box2);
  /*  world.add(
        make_shared<rectangle>(vec3(0, 555, 0), vec3(555, 555, 555), white));
    world.add(make_shared<rectangle>(vec3(0, 0, 0), vec3(555, 0, 555), white));
    world.add(
        make_shared<rectangle>(vec3(0, 0, 555), vec3(555, 555, 555), white));
      world.add(make_shared<rectangle>(vec3(555, 0, 0), vec3(555), green));
    world.add(make_shared<rectangle>(vec3(0, 0, 0), vec3(0, 555, 555), red));
    */
  bvh_node bvh(world);
  vec3 cameraPos(278, 278, -800);
  camera cam(40., RATIO, cameraPos, vec3(0, 1, 0), vec3(278, 278, 0), 0,
             (vec3(0, 0, -1) - cameraPos).length());

  baseIntegrator renderer(&bvh, IMG_HEIGHT, IMG_WIDTH, 200, &cam, vec3(0, 0, 0));
  renderer.Render();
  //renderer.RenderPxl(111,IMG_HEIGHT -1- 101,16,max_dep);
}
