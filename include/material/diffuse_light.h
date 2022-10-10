#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H

#include "material/material.h"
#include "math/baiscSampleFunctions.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "object/hitable.h"
#include "texture/constant_color.h"
#include "texture/texture.h"
#include <memory>

class diffuse_light : public material {
public:
  diffuse_light(std::shared_ptr<texture> _emit) : emit(_emit) {}
  diffuse_light(color light_color, const shared_ptr<material> &_mat)
      : emit(make_shared<constant_color>(light_color)), mat(_mat) {}
  bool scatter(const ray &_in, const record &hit_rec, color &out_attenuation,
               ray &out_scattered) const override {
    return false;
  }
  vec3 emitted(double u, double v, const vec3 &p) const override {
    return emit->value(u, v, p);
  }

  // ? ? ?
  vec3 f(const vec3 &w_o,const vec3& w_h,const vec3& w_i,
                          record &hit_rec) const override {
    return mat->f(w_o, w_h, w_i, hit_rec);
  }
  float pdf(const vec3 &wi, const vec3 &wh, const record &rec) const override {
    return mat->pdf(wi, wh, rec);
  }
  vec3 sample_f(const vec3 &wo, vec3 *wh, record &hit_rec) const override {
    return mat->sample_f(wo, wh, hit_rec);
  }

public:
  shared_ptr<texture> emit;
  shared_ptr<material> mat;
};

#endif
