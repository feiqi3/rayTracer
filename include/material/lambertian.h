/*
 * @Author: feiqi3
 * @Date: 2022-03-03 19:26:25
 * @LastEditTime: 2022-05-12 12:32:09
 * @LastEditors: feiqi3
 * @Description: |material lambertian|
 * @FilePath: \rayTracer\include\material\lambertian.h
 * ->blog: feiqi3.cn <-
 */

#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "material.h"
#include "../hitableList.h"
#include "../math/vector.h"
#include "../ray.h"
class lambertian : public material {
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &attenuation, ray &scattered) const override
                       {
                           vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
                           if (scatter_dir.is_close_to_zero()) {
                               scatter_dir = hit_rec.normal;
                           }
                           scattered = ray(hit_rec.p,scatter_dir);
                           attenuation = albedo;
                           return true;
                       }

    public:lambertian(const color& _albedo):albedo(_albedo){}

    public:color albedo;

};
#endif