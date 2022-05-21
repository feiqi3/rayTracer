/*
 * @Author: feiqi3
 * @Date: 2022-05-10 09:33:03
 * @LastEditTime: 2022-05-21 11:58:42
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\material\normal_shader.h
 * ->blog: feiqi3.cn <-
 */
#ifndef NORMAL_SHADER_H
#define NORMAL_SHADER_H

#include "material.h"
#include "../object/hitable.h"
class normal_shader : public material {
  bool scatter(const ray &_in, const record &hit_rec, color &out_attenuation,
               ray &out_scattered) const override {
    out_attenuation = ((hit_rec.normal) + 1) / 2;
    out_attenuation = out_attenuation / scale + (1 - divided_scale);
    vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
    if (scatter_dir.is_close_to_zero()) {
      scatter_dir = hit_rec.normal;
    }
    out_scattered = ray(hit_rec.p, scatter_dir);
    return 1;
  };

private:int scale;
        float divided_scale;
public:
  normal_shader():scale(1) {divided_scale = 1;}
  normal_shader(const int _scale):scale(_scale >= 1 ? _scale : 1){divided_scale = 1.0 /scale;}

    virtual vec3 getRayDir(const ray &_in, const record &hit_rec) const override{
    vec3 scatter_dir = hit_rec.normal + rand_in_unit_sphere();
    if (scatter_dir.is_close_to_zero()) {
      scatter_dir = hit_rec.normal;
    }
    return scatter_dir;
  }
};

#endif