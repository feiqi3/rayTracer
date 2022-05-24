/*
 * @Author: feiqi3
 * @Date: 2022-03-03 19:26:25
 * @LastEditTime: 2022-05-24 15:23:42
 * @LastEditors: feiqi3
 * @Description: |material lambertian|
 * @FilePath: \rayTracer\include\material\lambertian.h
 * ->blog: feiqi3.cn <-
 */

#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "../hitableList.h"
#include "../math/vector.h"
#include "../ray.h"
#include "material.h"

class lambertian : public material {
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
    if (scatter_dir.is_close_to_zero()) {
      scatter_dir = hit_rec.normal;
    }
    scattered = ray(hit_rec.p, scatter_dir);
    attenuation = albedo;
    if (material::emit) {
      return false;
    }
    return true;
  }

public:
  lambertian(const color &_albedo) {
     material::albedo = _albedo;
     material::ks = _albedo;
     material::ka = _albedo * vec3(.1,.1,.1);
     spec_lvl =8;
      }

public:
  virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const override {
    vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
    if (scatter_dir.is_close_to_zero()) {
      scatter_dir = hit_rec.normal;
    }
    return scatter_dir;
  }
};
#endif