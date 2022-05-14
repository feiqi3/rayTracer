/*
 * @Author: feiqi3
 * @Date: 2022-05-13 15:01:40
 * @LastEditTime: 2022-05-13 16:46:45
 * @LastEditors: feiqi3
 * @Description: |---Obj Triangle---|

 * @FilePath: \rayTracer\include\object\triangle.h
 * ->blog: feiqi3.cn <-
 */
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../math/vector.h"
#include "hitable.h"

class triangle : public hitable {
public:
  triangle(const vec3 &_p1, const vec3 &_p2, const vec3 &_p3,
           std::shared_ptr<material> _mat_ptr);
  vec3 getNormal() const;
  void resetTriangle(const vec3 &_p1, const vec3 &_p2, const vec3 &_p3);
  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;
  bool is_in_tri(const vec3 &_p) const;

private:
  vec3 p1, p2, p3;
  vec3 normal;
  /*
  these vectors are caculated during ctr
  and wil be used when determine whether
  the point is in the triangle or not
  */
  vec3 _v0, _v1;
  double _dot00, _dot01, _dot11;
};

inline triangle::triangle(const vec3 &_p1, const vec3 &_p2, const vec3 &_p3,
                          std::shared_ptr<material> _mat_ptr) 
                          :hitable(_mat_ptr),
                          p1(_p1),p2(_p2),p3(_p3)
{
    _v0 = p3 - p1;
    _v1 = p2 - p1;
    _dot00 = dot(_v0, _v0);
    _dot01 = dot(_v0, _v1);
    _dot11 = dot(_v1, _v1);
    normal = normalize(cross(_v1, p3 - p2));

}

inline bool triangle::is_in_tri(const vec3 &_p) const {
  vec3 _v2 = _p - p1;
  double dot02 = dot(_v0, _v2);
  double dot12 = dot(_v1, _v2);
  double inv_Deno = 1. / (_dot00 * _dot11 - _dot01 * _dot01);
  double u = (_dot11 * dot02 - _dot01 * dot12) * inv_Deno;
  if (u < 0 || u > 1) {
    return false;
  }
  double v = (_dot00 * dot12 - _dot01 * dot02) * inv_Deno;
  if (v < 0 || v > 1) {
    return false;
  }

  return u + v <= 1;
}

inline bool triangle::hit(const ray &r, double t_min, double t_max,
                          record &rec) const {
  double NdotP0cutO = dot(normal, p1 - r.orig);
  double NdotD = dot(normal, r.direction());
  double t = NdotP0cutO / NdotD;
  if (t < t_min || t > t_max) {
    return false;
  }
  vec3 pointP = r.at(t);
  if (!is_in_tri(pointP)) {
    return false;
  }
  rec.t = t;
  rec.p = pointP;
  rec.mat_ptr = mat_ptr;
  rec.normal = normal;
  set_face_normal(r, normal, rec);
  return true;
}
#endif