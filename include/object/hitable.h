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
#include "Macro.h"
#include "material/material.h"
#include "math/vector.h"
#include "ray.h"
#include <memory>

// Forward declear
class hitable;
class AABB;

typedef hitable object;
typedef int HType_t;

struct record;

class hitable {
public:
  enum Htype {
    Rect     = BIT(0),
    Box      = BIT(1),
    Sphere   = BIT(2),
    Triangle = BIT(3),
    Wrapper  = BIT(4),
    Set      = BIT(5),
    Other    = BIT(6)
  };

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const = 0;
  virtual bool bounding_box(AABB &box_out) const = 0;

  virtual material::MType getMType()const = 0; 

  virtual HType_t getType() const { return Other; }

//*Get sample point on the light, get pdf from current wi 
//*and get light's emission
  virtual vec3 getSample(record &rec, float *pdf,vec3 *emission) const = 0;
};

inline material::MType hitable::getMType()const{
  return material::Other;
}


inline vec3 hitable::getSample(record &rec, float *pdf,vec3 *emission) const { return 0; }

struct record {
  vec3 p;

  // normal MUST be normalized
  vec3 normal;
  std::shared_ptr<material> mat_ptr;
  double t;
  double u;
  double v;
  bool front_face;
  HType_t HitType;
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