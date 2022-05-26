/*
 * @Author: feiqi3
 * @Date: 2022-05-25 09:32:19
 * @LastEditTime: 2022-05-25 12:06:40
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\src\presets.cpp
 * ->blog: feiqi3.cn <-
 */
#ifndef PRESET_H_
#define PRESET_H_

#include "RenderQueue.h"
#include "hitableList.h"
#include "object/scene.h"
#include "object/sphere.h"
#include "material/dielectric.h"
#include "renderPass/camera.h"
#include "tool/fLoader.h"

namespace PreSets {
inline void House_under_Sky() {
  vec3 camPos(0, -1.2, 0);
  vec3 lookAt(-1, 2.5, -8);
  shared_ptr<renderPass> cam =
      make_shared<camera>(60, 16. / 9, camPos, vec3(0, 1, 0), lookAt, 0,
                          (lookAt - vec3(-1, 0, -10)).length());
  auto rq = renderQueue::getInstance();
  rq->setMainRender(cam);
  auto sc =
      std::make_shared<scene>(vec3(0, 0, 0), 4000, "./skyBox/Light Blue/");
  floader f;
  f.fload("model/others/cottage_obj.obj");
  auto mode = f.getModel("model/others/cottage_diffuse.png");
  mode->transform(mat::getScale(vec3(0.45, 0.45, 0.45)) *
                  mat::getTranslate(vec3(0, -5, -30)));
  auto SphereMetal = std::make_shared<sphere>(
      vec3(1, -1, -8), 1.15,
      std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.5));
  auto SphereDie = std::make_shared<sphere>(vec3(-5, -1, -6), 1,
                                            make_shared<dielectric>(1.5));
  rq->addObj(sc);
  rq->addObj(mode);
  rq->addObj(SphereMetal);
  rq->addObj(SphereDie);
}

inline void House_under_night() {
  vec3 camPos(0, -1.2, 0);
  vec3 lookAt(-1, 2.5, -8);
  shared_ptr<renderPass> cam =
      make_shared<camera>(60, 16. / 9, camPos, vec3(0, 1, 0), lookAt, 0,
                          (camPos - lookAt).length());
  auto rq = renderQueue::getInstance();
  rq->setMainRender(cam);
  auto sc =
      std::make_shared<scene>(vec3(0, 0, 0), 4000, "./skyBox/NeonSky/");
  floader f;
  f.fload("model/others/cottage_obj.obj");
  auto mode = f.getModel("model/others/cottage_diffuse.png");
  mode->transform(mat::getScale(vec3(0.45, 0.45, 0.45)) *
                  mat::getTranslate(vec3(0, -5, -30)));
  auto SphereMetal = std::make_shared<sphere>(
      vec3(1, -1, -8), 1.15,
      std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.5));
  auto SphereDie = std::make_shared<sphere>(vec3(-5, -1, -6), 1,
                                            make_shared<dielectric>(1.5));
  rq->addObj(sc);
  rq->addObj(mode);
}

inline void Three_Little_Ballgys() {
    vec3 camPos(0, 0, 5);
    vec3 lookAt(0, 0, -1);
    shared_ptr<renderPass> cam =
        make_shared<camera>(45, 16. / 9, camPos, vec3(0, 1, 0), lookAt, 1./13,
            (camPos - lookAt).length());
    auto rq = renderQueue::getInstance();
    rq->setMainRender(cam);
    auto sc =
   std::make_shared<scene>(vec3(0, 0, 0), 300, "./skyBox/Pandora [From tutorial]/");
    auto ball_left = std::make_shared<sphere>(vec3(-2, 0, -1), 1, std::make_shared<metal>(vec3(0.05, 0.13, 0.22)* 1.7, 0.3));
    auto ball_mid = std::make_shared<sphere>(vec3(0, 0, -1), 1, std::make_shared<lambertian>(vec3(0.1, 0.1, 0.211)));
    auto ball_right = std::make_shared<sphere>(vec3(2, 0, -1), 1, std::make_shared<dielectric>(1.3));
    auto ball_ground = std::make_shared<sphere>(vec3(0, -99, -1), 98, std::make_shared<metal>(vec3(0.1, 0.4, 0.23), 0.03));
    rq->addLight(make_shared<sphere>(vec3(-1, 4, 5), 0.3,std::make_shared<lambertian>(vec3(0.36, 0.55, 0.72))), vec3(0.5, 0.5, 0.5) * 1.4);
    rq->addObj(ball_left);
    rq->addObj(ball_mid);
    rq->addObj(ball_right);
    rq->addObj(ball_ground);
    rq->addObj(sc);
}

} // namespace PreSets

#endif