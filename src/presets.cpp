/*
 * @Author: feiqi3
 * @Date: 2022-05-25 09:32:19
 * @LastEditTime: 2022-05-25 12:06:40
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\src\presets.cpp
 * ->blog: feiqi3.cn <-
 */
#include "RenderQueue.h"
#include "hitableList.h"
#include "object/scene.h"
#include "object/sphere.h"
#include "material/dielectric.h"
#include "renderPass/camera.h"
#include "tool/fLoader.h"

namespace PreSets {
void House_under_Sky() {
  vec3 camPos(0, -1.2, 0);
  vec3 lookAt(-1, 2.5, -8);
  shared_ptr<renderPass> cam =
      make_shared<camera>(60, 16. / 9, camPos, vec3(0, 1, 0), lookAt, 0,
                          (lookAt - vec3(-1, 0, -10)).length());
  auto rq = renderQueue::getInstance();
  rq->setMainRender(cam);
  auto sc =
      std::make_shared<scene>(vec3(0, 0, 0), 4000, "./skyBox/Light Blue/");
 auto sphereLight = std::make_shared<sphere>(vec3(0,30,0),0.1,std::make_shared<lambertian>(color(0.7, 0.3, 0.3)));
  rq->addLight(sphereLight, vec3(0.3, 0.3, 0.3) * 0.5);
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
} // namespace PreSets