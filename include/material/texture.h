/*
 * @Author: feiqi3
 * @Date: 2022-03-03 19:26:25
 * @LastEditTime: 2022-05-16 21:47:12
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
    if (tri == nullptr) {
      attenuation = color(0, 0, 0);
    } else {
      barycoord coord;
      coord = tri->get_barycentric(hit_rec.p);
      vec3 sample_coord = coord.alpha * tri->texP1() +
                          coord.beta * tri->texP2() +
                          coord.gamma * tri->texP3();
      attenuation = buffer.sampler(sample_coord.x(), sample_coord.y()) / 255.0;
    }
    scattered = ray(hit_rec.p, scatter_dir);
    return true;
  }

public:
  texture(const RGB12 &_buf) : buffer(_buf), tri(nullptr) {}
  void set_triangle(std::shared_ptr<triangle>_tri) { tri = _tri; }

private:
  RGB12 buffer;
  std::shared_ptr<triangle> tri;

protected:
  bool is_down_triangle;
};
#endif