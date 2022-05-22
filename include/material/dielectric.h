/*
 * @Author: feiqi3
 * @Date: 2022-03-17 09:44:16
 * @LastEditTime: 2022-05-22 10:02:34
 * @LastEditors: feiqi3
 * @Description: |Dielectric material class,like glass|
 * @FilePath: \rayTracer\include\material\dielectric.h
 * ->blog: feiqi3.cn <-
 */

#include "../math/vector.h"
#include "../object/hitable.h"
#include "../ray.h"
#include "../tool/common.h"
#include "material.h"
#include "math/vector4.h"

#include <cmath>
#include <stdlib.h>
#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

class dielectric : public material {
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &out_attenuation,
                       ray &out_scattered) const override {
    out_attenuation = albedo;
    double refraction_ratio = hit_rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = normalize(_in.direction());
    double cos_theta = fmin(dot(-unit_direction, hit_rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > rand_d())
      direction = reflection(unit_direction, hit_rec.normal);
    else
      direction = refract(unit_direction, hit_rec.normal, refraction_ratio);
    out_scattered = ray(hit_rec.p, direction);
    return true;
  }

public:
  float ir; // the index of refactor
public:
  dielectric(float index_of_refractor,color _albedo = vec3(1.0,1.0,1.0)) : ir(index_of_refractor) {material::albedo = _albedo;}

private:
  static double reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }

public:
  virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const override {
    double refraction_ratio = hit_rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = normalize(_in.direction());
    double cos_theta = fmin(dot(-unit_direction, hit_rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > rand_d())
      direction = reflection(unit_direction, hit_rec.normal);
    else
      direction = refract(unit_direction, hit_rec.normal, refraction_ratio);
    return direction;
  }

};
#endif