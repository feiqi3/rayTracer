/*
 * @Author: feiqi3
 * @Date: 2022-03-03 19:26:25
 * @LastEditTime: 2022-05-22 10:17:26
 * @LastEditors: feiqi3
 * @Description: |material lambertian|
 * @FilePath: \rayTracer\include\material\texture.h
 * ->blog: feiqi3.cn <-
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../buffer/Buffer.h"
#include "../buffer/RGB12Buffer.h"
#include "../hitableList.h"
#include "../math/vector.h"
#include "../object/hitable.h"
#include "../object/triangle.h"
#include "../ray.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include <iostream>
#include <memory>

class texture : public material {
public:
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    vec3 scatter_dir;

    if (tri.lock() == nullptr) {
      attenuation = color(0, 0, 0);
    } else {
      barycoord coord;
      coord = tri.lock()->get_barycentric(hit_rec.p, hit_rec);
      vec3 sample_coord = coord.alpha * tri.lock()->texP1() +
                          coord.beta * tri.lock()->texP2() +
                          coord.gamma * tri.lock()->texP3();
      if (sample_coord.y() > 1)
        int x = 3;

      // When it doesn't have a picture buffer,
      // use material's albedo instead
      if (_buffer != nullptr)
        attenuation = _buffer->sampler(sample_coord.x(), sample_coord.y()) *
                      material::albedo;
      else
        attenuation = material::albedo;
    }
    if (_mat->is_light == true)
      return false;
    scatter_dir = _mat->getRayDir(_in, hit_rec);
    scattered = ray(hit_rec.p, scatter_dir);
    return true;
  }

public:
  texture(const std::shared_ptr<RGB12> &_buf = nullptr,
          shared_ptr<material> mat = std::make_shared<lambertian>(vec3(1, 1,
                                                                       1)),
          bool _light = false)
      : _buffer(_buf), _mat(mat) {
    material::is_light = _light;
    material::albedo = _mat->albedo;
    material::setLightandColor(mat->is_light, mat->emit);
  }

  void set_triangle(const std::shared_ptr<triangle> &_tri) { tri = _tri; }

  virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const override {
    vec3 scatter_dir;
    if (_mat == nullptr) {
      lambertian tmp(vec3(0, 0, 0));
      scatter_dir = tmp.getRayDir(_in, hit_rec);
    } else {
      scatter_dir = _mat->getRayDir(_in, hit_rec);
    }
    return scatter_dir;
  }

protected:
  std::weak_ptr<triangle> tri;
  std::shared_ptr<RGB12> _buffer;
  std::shared_ptr<material> _mat;
};
#endif