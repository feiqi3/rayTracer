/*
 * @Author: feiqi3
 * @Date: 2022-03-02 13:44:22
 * @LastEditTime: 2022-03-03 20:32:52
 * @LastEditors: feiqi3
 * @Description: |material class|
 * @FilePath: \rayTracer\include\material\material.h
 * ->blog: feiqi3.cn <-
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/vector.h"
#include "ray.h"
struct record;


class material {
public:
    //deprecated
  virtual bool scatter(const ray &_in, const record &hit_rec,
                       color &out_attenuation, ray &out_scattered) const = 0;

  enum MType{
  Lambertian,
  Microfacet,
  Light,
  Other
  };

//u v is the texcoords on the Light,
//p_on_light is the point ray hit on light, and p_on_pre_surf is the point on the previous surface.  
//Some kind of Light source may need those information  
    virtual vec3 emitted(double u, double v, const vec3 &p_on_light,const vec3&p_on_pre_surf) const {
    return vec3(0);
  }
  virtual vec3 f(const vec3 &w_o,const vec3& w_h,const vec3& w_i,
                          record &hit_rec)  = 0;
  virtual float pdf(const vec3 &wi, const vec3& wh, const record &rec) const = 0;
  virtual vec3 sample_f(const vec3& wo,vec3* wh,record &hit_rec) const = 0;
  virtual MType getType()const{
    return Other;
  }

};

#endif