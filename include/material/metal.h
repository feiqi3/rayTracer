/*
 * @Author: feiqi3
 * @Date: 2022-03-03 20:14:32
 * @LastEditTime: 2022-03-03 22:04:31
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
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override {
    auto reflect_dir = reflection(_in.dir, hit_rec.normal);
    scattered = ray(hit_rec.p, reflect_dir);
    attenuation = albedo;
    return (dot(scattered.dir,hit_rec.normal) > 0);
  }

    public:metal(const color& _albedo):albedo(_albedo){}
public:

public:
  color albedo;


};
#endif