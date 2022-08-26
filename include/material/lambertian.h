/*
 * @Author: feiqi3
 * @Date: 2022-03-03 19:26:25
 * @LastEditTime: 2022-03-03 22:04:19
 * @LastEditors: feiqi3
 * @Description: |material lambertian|
 * @FilePath: \rayTracer\include\material\lambertian.h
 * ->blog: feiqi3.cn <-
 */

#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "hitableList.h"
#include "material.h"
#include "math/vector.h"
#include "ray.h"
#include "texture/constant_color.h"
#include "texture/texture.h"
#include <memory>

class lambertian : public material {
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
    if (scatter_dir.is_close_to_zero()) {
      scatter_dir = hit_rec.normal;
    }
    scattered = ray(hit_rec.p, scatter_dir);
    attenuation = albedo->value(hit_rec.u, hit_rec.v, hit_rec.p);
    return true;
  }

public:
  lambertian(const std::shared_ptr<texture> &_albedo) : albedo(_albedo) {}
  lambertian(const vec3& color) : albedo(std::make_shared<constant_color>(color)) {}
public:
  std::shared_ptr<texture> albedo;
};
#endif