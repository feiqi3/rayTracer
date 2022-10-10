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

#include "math/baiscSampleFunctions.h"
#include "hitableList.h"
#include "material.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "object/hitable.h"
#include "ray.h"
#include "texture/constant_color.h"
#include "texture/texture.h"
#include "tool/common.h"
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
  lambertian(const vec3 &color)
      : albedo(std::make_shared<constant_color>(color)) {}
  float pdf(const vec3 &wi, const vec3& wh, const record &rec) const override;
  vec3 f(const vec3 &w_o,const vec3& w_h,const vec3& w_i,
                          record &hit_rec) const override;
  vec3 sample_f(const vec3& wo,vec3* wh,record &hit_rec) const override;
public:
  std::shared_ptr<texture> albedo;
};

//cos-weighted
inline float lambertian::pdf(const vec3 &wi, const vec3& wh,
                            const record &rec) const {
  return absDot(wi,rec.normal) * inv_pi;
}

//cos-weighted
inline vec3 lambertian::f(const vec3 &w_o,const vec3& w_h,const vec3& w_i,
                          record &hit_rec) const {                          
  return albedo->value(hit_rec.u, hit_rec.v, hit_rec.p) * inv_pi;
}

inline vec3 lambertian::sample_f(const vec3& wo,vec3* wh,record &hit_rec)const{
  auto tbn = mat::getTBN(hit_rec.normal);
  vec3 rVec = sampleCosWeighted() * tbn;
  *wh = normalize(rVec - wo);
  return rVec;
}

#endif