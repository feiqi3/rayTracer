#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "material/material.h"
#include "math/vector.h"
#include "object/aabb.h"
#include "object/hitable.h"
#include "tool/common.h"
#include <algorithm>
#include <cmath>
#include <memory>
class triangle : public hitable {

public:
  triangle() {}
  // pa： upper left, pc: lower right
  triangle(const vec3 &pa, const vec3 &pb, const vec3 &pc,
           const std::shared_ptr<material> &_mat,
           bool clock_wise_texcoord = false)
      : mat_ptr(_mat), point{pa, pb, pc}, texcoord{vec3(0, 1, 0),
                                                   clock_wise_texcoord
                                                       ? vec3(1, 1, 0)
                                                       : vec3(0),
                                                   vec3(1, 0, 0)} {
    _data_caculate();
  }
  bool is_in_tri(const vec3 &_p) const;
  bool bounding_box(AABB &box_out) const override;
  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;
  std::shared_ptr<material> mat_ptr;
  HType_t getType() const override { return Htype::Triangle; }
  material::MType getMType() const override{
    return mat_ptr->getType();
  }
  vec3 getSample(record &rec, float *pdf,vec3* emission) const override;
  void get_triangle_uv(const vec3 &p, double &u, double &v) const;

public:
  vec3 normal;
  float area;

protected:
  vec3 point[3];
  vec3 texcoord[3];
  vec3 _v0, _v1;
  double _dot00, _dot01, _dot11;

  float inv_area;
  vec3 center;
  double getTriangleSize(const vec3 &p0, const vec3 &p1, const vec3 &p2) const;
  void _data_caculate();
};

inline double triangle::getTriangleSize(const vec3 &p0, const vec3 &p1,
                                        const vec3 &p2) const {
  vec3 ab = p1 - p0;
  vec3 ac = p2 - p0;
  return 0.5 * cross(ab, ac).length();
}

inline void triangle::_data_caculate() {
  center = point[1]+point[2]+point[0];
  center =center/3;
  _v0 = point[2] - point[0];
  _v1 = point[1] - point[0];
  _dot00 = dot(_v0, _v0);
  _dot01 = dot(_v0, _v1);
  _dot11 = dot(_v1, _v1);
  normal = normalize(cross(_v1, point[2] - point[1]));
  area = getTriangleSize(point[0], point[1], point[2]);
}

inline bool triangle::hit(const ray &r, double t_min, double t_max,
                          record &rec) const {
  double NdotP0cutO = dot(normal, point[0] - r.orig);
  double NdotD = dot(normal, r.direction());
  double t = NdotP0cutO / NdotD;
  if (t < t_min || t > t_max)
    return false;
  vec3 pointP = r.at(t);
  if (!is_in_tri(pointP))
    return false;
  rec.t = t;
  rec.p = pointP;
  get_triangle_uv(rec.p, rec.u, rec.v);
  rec.mat_ptr = mat_ptr;
  rec.normal = normal;
  set_face_normal(r, normal, rec);
  rec.HitType = this->getType();
  return true;
}

inline bool triangle::is_in_tri(const vec3 &_p) const {
  vec3 _v2 = _p - point[0];
  double dot02 = dot(_v0, _v2);
  double dot12 = dot(_v1, _v2);
  double inv_Deno = 1. / (_dot00 * _dot11 - _dot01 * _dot01);
  double u = (_dot11 * dot02 - _dot01 * dot12) * inv_Deno;
  if (u < 0 || u > 1)
    return false;
  double v = (_dot00 * dot12 - _dot01 * dot02) * inv_Deno;
  if (v < 0 || v > 1)
    return false;
  return u + v <= 1;
}

inline bool triangle::bounding_box(AABB &box_out) const {
  vec3 pMin =
      vec3(std::min(std::min(point[0].x(), point[1].x()), point[2].x()),
           std::min(std::min(point[0].y(), point[1].y()), point[2].y()),
           std::min(std::min(point[0].z(), point[1].z()), point[2].z())) -
      vec3(0.00001);
  vec3 pMax =
      vec3(std::max(std::max(point[0].x(), point[1].x()), point[2].x()),
           std::max(std::max(point[0].y(), point[1].y()), point[2].y()),
           std::max(std::max(point[0].z(), point[1].z()), point[2].z())) +
      vec3(0.00001);
  box_out = AABB(pMin, pMax);
  return true;
}

inline void triangle::get_triangle_uv(const vec3 &p, double &u,
                                      double &v) const {
  auto alpha = getTriangleSize(point[2], point[1], p) / area;
  auto beta = getTriangleSize(point[2], point[0], p) / area;
  auto gamma = 1 - alpha - beta;
  vec3 uv = alpha * texcoord[0] + beta * texcoord[1] + gamma * texcoord[2];
  u = uv.x();
  v = uv.y();
}

inline vec3 triangle::getSample(record &rec, float *pdf,vec3* emission) const {
  float u = std::sqrt(rand_d());
  float v = rand_d() * u;
  u = 1 - u;
  float w = 1 - u - v;
  vec3 samplePoint = (point[0] * u + point[1] * v + point[2] * w);
  vec3 sampleVec = samplePoint - rec.p;
  float d2 = (sampleVec-rec.p).lengthSquare();
  sampleVec = normalize(sampleVec);
  *pdf =(area * absDot(sampleVec, normal))/d2; 
  vec3 uv = u * texcoord[0] + v*texcoord[1] +w*texcoord[2];
  *emission = mat_ptr->emitted(uv.x(), uv.y(), samplePoint, rec.p);
  return samplePoint;
}
#endif