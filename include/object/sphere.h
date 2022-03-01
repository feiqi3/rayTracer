/*
 * @Author: feiqi3
 * @Date: 2022-01-30 11:46:53
 * @LastEditTime: 2022-02-02 11:21:57
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\object\sphere.h
 * ->blog: feiqi3.cn <-
 */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "hitable.h"
#include "math/vector.h"

class sphere : public hitable {
private:
  vec3 cen;
  double radius;

public:
  sphere() {}
  sphere(vec3 center, double r) : cen(center), radius(r) {}

  // setter and getter
  vec3 getCenter() const { return cen; }
  double getRadius() const { return radius; }
  void setCenter(const vec3 center) { cen = center; }
  void setRadius(const double r) { radius = r; }

  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override {
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

    rec.normal = (rec.p - cen) / radius; // normalized
    set_face_normal(r, rec.normal, rec);

    return true;
  }
};

#endif