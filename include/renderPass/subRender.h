/*
 * @Author: feiqi3
 * @Date: 2022-05-10 13:54:41
 * @LastEditTime: 2022-05-18 14:00:22
 * @LastEditors: feiqi3
 * @Description: |---
 Actually I want to make a hybrid rayTracer/renderer
 Maybe I can start it at getting normal/depth first?
 Then go the shadow.
 ---|
 * @FilePath: \rayTracer\include\renderPass\subRender.h
 * ->blog: feiqi3.cn <-
 */
#ifndef SUBRENDER_H
#define SUBRENDER_H

#include "../math/vector.h"
#include "../ray.h"
#include "camera.h"
#include "renderPass/renderPass.h"
class NormalRender : public renderPass {
public:
  NormalRender(camera cam)
      : renderPass(cam.fov_vertical, cam.ratio, cam.origin, cam.up,
                   cam.look_at,RenderType::SubRender) {
    horizontal = viewport_width * renderPass::u;
    vertical = viewport_height * renderPass::v;
    lower_left_corner =
        renderPass::origin - horizontal / 2 - vertical / 2 - renderPass::w;

  Flog::flog(TRACE, toString());
    Flog::flog(TRACE, "U:" + u.toString() + ",V:" + v.toString() +
                          ",W:" + w.toString() +
                          ",Lower left corner:" + lower_left_corner.toString()+"\n");
  
  }

  GET_CLASS_NAME(NormalRender);
  const std::string toString() const override
  {
    std::string classname = clsname();
    return classname+","+"Position: "+origin.toString()+",look at: "+look_at.toString()+".";
  }

  ray get_ray(double s, double t) override {
    return ray(origin,
               lower_left_corner + s * horizontal + t * vertical - origin);
  }
  virtual color cast_ray(const ray &r, const hitable_list &world,
                         int depth) override {
    record rec;
    if (world.hit(r, 0.001, Infinity, rec)) {
      return rec.normal;
    }
    return vec3(0, 0, 0);
  }

protected:
  vec3 lower_left_corner, vertical, horizontal;
};

class DepthRender : public renderPass {
public:
  DepthRender(camera cam)
      : renderPass(cam.fov_vertical, cam.ratio, cam.origin, cam.up,
                   cam.look_at,RenderType::SubRender) {
    horizontal = viewport_width * renderPass::u;
    vertical = viewport_height * renderPass::v;
    lower_left_corner =
        renderPass::origin - horizontal / 2 - vertical / 2 - renderPass::w;
  
  Flog::flog(INFO, toString());
    Flog::flog(TRACE, "U:" + u.toString() + ",V:" + v.toString() +
                          ",W:" + w.toString() +
                          ",Lower left corner:" + lower_left_corner.toString()+"\n");
  }
  GET_CLASS_NAME(DepthRender);
  const std::string toString() const override
  {
    std::string classname = clsname();
    return classname+","+"Position: "+origin.toString()+",look at: "+look_at.toString()+".";
  }

  ray get_ray(double s, double t) override {
    return ray(origin,
               lower_left_corner + s * horizontal + t * vertical - origin);
  }
  virtual color cast_ray(const ray &r, const hitable_list &world,
                         int depth) override {
    record rec;
    if (world.hit(r, 0.001, Infinity, rec)) {
      float len = (rec.t * r.dir).length();
      return vec3(len,len,len);
    }
    return vec3(Infinity, Infinity, Infinity);
  }

protected:
  vec3 lower_left_corner, vertical, horizontal;
};

#endif
