/*
 * @Author: feiqi3
 * @Date: 2022-01-30 11:46:53
 * @LastEditTime: 2022-03-29 14:38:25
 * @LastEditors: feiqi3
 * @Description: |obj class|
 * @FilePath: \rayTracer\include\object\sphere.h
 * ->blog: feiqi3.cn <-
 */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "aabb.h"
#include "hitable.h"
#include "math/vector.h"
#include <memory>
#include <utility>

class sphere : public hitable {
private:
  vec3 cen;
  double radius;

public:
  sphere() {}
  sphere(vec3 center, double r, std::shared_ptr<material> _material)
      : cen(center), radius(r), mat_ptr(_material) {}

  std::shared_ptr<material> mat_ptr;
  // setter and getter
  vec3 getCenter() const { return cen; }
  double getRadius() const { return radius; }
  void setCenter(const vec3 center) { cen = center; }
  void setRadius(const double r) { radius = r; }

  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;

  bool bounding_box(AABB &box_out) const override;
};

inline bool sphere::hit(const ray &r, double t_min, double t_max,
                        record &rec) const {
  vec3 o2o = r.orig - cen;
  double a = dot(r.direction(), r.direction());
  double half_b = dot(r.direction(), (o2o));
  double c = dot(o2o, o2o) - radius * radius;
  double dis = half_b * half_b - a * c;
  if (dis < 0) {
    return false;
  }
  double division_a = 1 / a;
  double sqrt_dis = sqrt(dis);
  double root = (-half_b - sqrt_dis) * division_a;
  if (root > t_max || root < t_min) {
    root = (-half_b - sqrt_dis) * division_a;
    if (root > t_max || root < t_min) {
      return false;
    }
  }
  rec.t = root;
  rec.p = r.at(rec.t);
  rec.mat_ptr = mat_ptr;
  rec.normal = (rec.p - cen) / radius; // normalized
  set_face_normal(r, rec.normal, rec);
  return true;
}

inline bool sphere::bounding_box(AABB &box_out) const {
  box_out = AABB(getCenter() - vec3(radius), getCenter() - vec3(radius));
  return true;
}


#endif