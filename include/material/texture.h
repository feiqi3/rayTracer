/*
 * @Author: feiqi3
 * @Date: 2022-03-03 19:26:25
 * @LastEditTime: 2022-05-18 14:01:40
 * @LastEditors: feiqi3
 * @Description: |material lambertian|
 * @FilePath: \rayTracer\include\material\texture.h
 * ->blog: feiqi3.cn <-
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../hitableList.h"
#include "../math/vector.h"
#include "../ray.h"
#include "../buffer/Buffer.h"
#include "../buffer/RGB12Buffer.h"
#include "material.h"
#include "../object/hitable.h"
#include "../object/triangle.h"
#include <iostream>
#include <memory>

class texture : public material {
  public:
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
    if (scatter_dir.is_close_to_zero()) {
      scatter_dir = hit_rec.normal;
    }
    if (tri.lock() == nullptr) {
      attenuation = color(0, 0, 0);
    } else {
      barycoord coord;
      coord = tri.lock()->get_barycentric(hit_rec.p,hit_rec);
      vec3 sample_coord = coord.alpha * tri.lock()->texP1() +
                          coord.beta * tri.lock()->texP2() +
                          coord.gamma * tri.lock()->texP3();
      attenuation = _buffer->sampler(sample_coord.x(), sample_coord.y());
    }
    scattered = ray(hit_rec.p, scatter_dir);
    return true;
  }

public:
  texture(const std::shared_ptr<RGB12>& _buf) : _buffer(_buf) {}

  void set_triangle(const std::shared_ptr<triangle>&_tri) {
     tri = _tri; }
  
protected:
  std::weak_ptr<triangle> tri;
  std::shared_ptr<RGB12> _buffer;
};
#endif