/*
 * @Author: feiqi3
 * @Date: 2022-03-02 13:44:22
 * @LastEditTime: 2022-05-24 13:45:29
 * @LastEditors: feiqi3
 * @Description: |material class|
 * @FilePath: \rayTracer\include\material\material.h
 * ->blog: feiqi3.cn <-
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../math/vector.h"
#include "../ray.h"
#include <memory>
struct record;

class material{
public:
  material() : albedo(0, 0, 0) {}
  virtual vec3 emitted() const { return emit_; }
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &out_attenuation, ray &out_scattered) const = 0;
  virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const = 0;

  virtual void setEmitandColor(vec3 color) {
    is_light = false;
    emit = true;
    emit_ = color;
  };

  virtual void setLightandColor(vec3 colr)
  {
    is_light = true;
    emit = false;
    emit_ = colr;
  }

  bool emit = false;
  bool is_light = false;
  vec3 emit_ = vec3(0, 0, 0);
  color albedo;//kd
  vec3 ka;
  vec3 ks;
  int spec_lvl;
};

#endif