/*
 * @Author: feiqi3
 * @Date: 2022-03-03 20:14:32
 * @LastEditTime: 2022-03-04 10:45:40
 * @LastEditors: feiqi3
 * @Description: |metal material|
 * @FilePath: \rayTracer\include\material\metal.h
 * ->blog: feiqi3.cn <-
 */
#ifndef METAL_H_
#define METAL_H_

#include "hitableList.h"
#include "material.h"
#include "math/vector.h"
#include "ray.h"

class metal : public material {
public:
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    auto reflect_dir =
        reflection(_in.dir, hit_rec.normal) + fuzz * rand_in_unit_sphere();
    scattered = ray(hit_rec.p, reflect_dir);
    attenuation = albedo;
    return (dot(scattered.dir, hit_rec.normal) > 0);
  }
  // When fuzz is 0 is total reflection

  metal(const color &_albedo, float _fuzz)
      : albedo(_albedo), fuzz(_fuzz > 1 ? 1 : _fuzz) {}

public:
  color albedo;
  float fuzz;
};
#endif