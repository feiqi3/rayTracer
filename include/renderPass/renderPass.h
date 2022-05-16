/*
 * @Author: feiqi3
 * @Date: 2022-05-10 13:38:05
 * @LastEditTime: 2022-05-15 12:49:51
 * @LastEditors: feiqi3
 * @Description: |---
 Render pass base class 
 Ray casting can do everything
 ---|
 * @FilePath: \rayTracer\include\renderPass\renderPass.h
 * ->blog: feiqi3.cn <-
 */

#ifndef RENDERPASS_H
#define RENDERPASS_H
#include "../Macro.h"
#include "../hitableList.h"
#include "../math/vector.h"
#include "../ray.h"
#include <iostream>

enum RenderType {
  MainRender = BIT(0),
  PostEffect = BIT(1),
  LightRender = BIT(2),
  SubRender = BIT(3)
};


class renderPass {
public:
//need basic camera infomation to build this pass
  renderPass(float _fov, float _ratio, vec3 pos, vec3 up, vec3 look_at);
  virtual ~renderPass();
  //get ray from viewport
  virtual ray get_ray(double s, double t) = 0;
  //get ray's feedback
  virtual color cast_ray(const ray &r, const hitable_list &world, int depth) = 0;

  virtual const std::string toString() const = 0;
public:
  const vec3 origin;
  const vec3 up;
  const vec3 look_at;
  const float fov_vertical;
  const float ratio;
  const float viewport_height;
  const float viewport_width;

protected:
  vec3 lookDir;
  vec3 u, v, w;
  //Like lookat Matrix in glm
};
inline renderPass::renderPass(float _fov, float _ratio, vec3 pos, vec3 _up,
                              vec3 lookat)
    : origin(pos), up(_up), look_at(lookat), fov_vertical(_fov), ratio(_ratio),
      viewport_height(2 * tan(d2radian(fov_vertical / 2.0))),
      viewport_width(ratio * viewport_height) {
  lookDir = renderPass::origin - renderPass::look_at;
  w = normalize(lookDir);
  u = normalize(cross(up, w));
  v = cross(w, u);


  
}
inline renderPass::~renderPass() {}
#endif