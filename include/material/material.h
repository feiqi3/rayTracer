/*
 * @Author: feiqi3
 * @Date: 2022-03-02 13:44:22
 * @LastEditTime: 2022-05-22 10:45:57
 * @LastEditors: feiqi3
 * @Description: |material class|
 * @FilePath: \rayTracer\include\material\material.h
 * ->blog: feiqi3.cn <-
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../math/vector.h"
#include "../ray.h"
struct record;

class material {
public:
  material() : albedo(0, 0, 0) {}
  virtual vec3 emitted() const { return emit; }
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &out_attenuation, ray &out_scattered) const = 0;
  virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const = 0;
  color albedo;
  virtual void setLightandColor(bool light, vec3 color) {
    is_light = light;
    emit = color;
  };
  bool is_light = false;
  vec3 emit = vec3(0, 0, 0);
};

#endif