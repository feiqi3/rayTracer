/*
 * @Author: feiqi3
 * @Date: 2022-01-28 18:18:09
 * @LastEditTime: 2022-05-24 12:06:51
 * @LastEditors: feiqi3
 * @Description: |Ray hit class|
 * @FilePath: \rayTracer\include\object\hitable.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _HITABLE_H_
#define _HITABLE_H_
#include "../Macro.h"
#include "../math/vector.h"
#include "../ray.h"
#include "../tool/flog.h"
#include "../material/material.h"
#include <memory>

class material;

class hitable;

typedef hitable object;

struct record {
  vec3 p;

  // normal should be normalized
  vec3 normal;
  std::shared_ptr<material> mat_ptr;
  double t;
  bool front_face;
};

inline void set_face_normal(const ray &r, const vec3 &v_out_normal,
                            record &rec) {
  if (dot(r.dir, v_out_normal) < 0.0) {
    rec.front_face = true;
  } else {
    rec.front_face = false;
    rec.normal = -rec.normal;
  }
}

class hitable{
public:
  hitable(std::shared_ptr<material> _mat_ptr) : mat_ptr(_mat_ptr) {}
  hitable() {}
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const = 0;
  virtual const std::string toString() const = 0;
  vec3 center_p;
  std::shared_ptr<material> mat_ptr;
};

#endif