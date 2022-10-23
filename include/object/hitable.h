/*
 * @Author: feiqi3
 * @Date: 2022-01-28 18:18:09
 * @LastEditTime: 2022-03-29 14:38:06
 * @LastEditors: feiqi3
 * @Description: |Ray hit class|
 * @FilePath: \rayTracer\include\object\hitable.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _HITABLE_H_
#define _HITABLE_H_
#include "material/material.h"
#include "math/vector.h"
#include "ray.h"
#include <memory>

// Forward declear
class hitable;
class AABB;

typedef hitable object;

struct record;

class hitable {
public:
  enum Htype { Rect, Box, Sphere, Triangle, Other };

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const = 0;
  virtual bool bounding_box(AABB &box_out) const = 0;

  virtual Htype getType() const { return Other; }
};

struct record {
  vec3 p;

  // normal MUST be normalized
  vec3 normal;
  std::shared_ptr<material> mat_ptr;
  double t;
  double u;
  double v;
  bool front_face;
  hitable::Htype HitType;
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
#endif