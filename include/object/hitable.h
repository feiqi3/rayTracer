/*
 * @Author: feiqi3
 * @Date: 2022-01-28 18:18:09
 * @LastEditTime: 2022-03-03 12:36:34
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\object\hitable.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _HITABLE_H_
#define _HITABLE_H_
#include "math/vector.h"
#include "ray.h"
#include <memory>

class hitable;

class material;
          
typedef hitable object;

struct record {
  vec3 p;
  vec3 normal;
  std::shared_ptr<material> mat_ptr;
  double t;
  bool front_face;
};

inline void set_face_normal(const ray& r,const vec3& v_out_normal,record& rec)
{
  if(dot(r.dir,v_out_normal)<0.0)
  {
    rec.front_face = true;
  }
  else {
  rec.front_face = false;
  rec.normal = - rec.normal;
  }
}

class hitable {
public:
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const = 0;
};

#endif