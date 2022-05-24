/*
 * @Author: feiqi3
 * @Date: 2022-03-03 20:14:32
 * @LastEditTime: 2022-05-24 15:24:10
 * @LastEditors: feiqi3
 * @Description: |metal material|
 * @FilePath: \rayTracer\include\material\metal.h
 * ->blog: feiqi3.cn <-
 */
#ifndef METAL_H_
#define METAL_H_

#include "../hitableList.h"
#include "../math/vector.h"
#include "../ray.h"
#include "material.h"

class metal : public material {
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    vec3 reflect_dir =
        reflection(_in.dir, hit_rec.normal) + fuzz * rand_in_unit_sphere();
    scattered = ray(hit_rec.p, reflect_dir);
    attenuation = albedo;
    if (material::is_light) {
      return false;
    }
    return (dot(scattered.dir, hit_rec.normal) > 0);
  }
  // When fuzz is 0 is total reflection
public:
  metal(const color &_albedo, float _fuzz) : fuzz(_fuzz > 1 ? 1 : _fuzz) {
    material::albedo = _albedo;
    spec_lvl = (1.26 - fuzz) * 64;
    material::ka = _albedo * vec3(.1,.1,.1);
    material::ks = albedo;
  }
  virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const override {
    vec3 reflect_dir =
        reflection(_in.dir, hit_rec.normal) + fuzz * rand_in_unit_sphere();
    return reflect_dir;
  }

public:
  float fuzz;
};
#endif