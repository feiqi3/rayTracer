#ifndef AARECT_H_
#define AARECT_H_

#include "aabb.h"
#include "material/material.h"
#include "math/vector.h"
#include "object/hitable.h"
#include "object/triangle.h"
#include "tool/common.h"
#include <cmath>
#include <cstdlib>

class xz_rect : public hitable {
public:
  xz_rect() {}

  xz_rect(double _x0, double _x1, double _z0, double _z1, double _k,
          shared_ptr<material> mat)
      : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k),
        upper(vec3(_x0, _k, _z1), vec3(_x1, _k, _z1), vec3(_x1, _k, _z0), mat,
              true),
        lower(vec3(_x0, _k, _z1), vec3(_x0, _k, _z0), vec3(_x1, _k, _z0), mat),
        mp(mat){};

  HType_t getType() const override { return Rect; }
  material::MType getMType() const override { return upper.getMType(); }
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;

  virtual bool bounding_box(AABB &output_box) const override {
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    output_box = AABB(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
    return true;
  }
  vec3 getSample(record &rec, float *pdf, vec3 *emission) const override {
    vec3 samplePoint = vec3(rand_d(x0, x1), k, rand_d(z0, z1));
    float d2 = (samplePoint - rec.p).lengthSquare();
    float area = abs((x1 - x0) * (z1 - z0));
    double dd = absDot(normalize(samplePoint - rec.p), upper.normal);
    double _pdf =  area * dd/d2;
    *pdf = static_cast<float>(_pdf);
    double uu, vv;
    if (upper.is_in_tri(samplePoint)) {
      upper.get_triangle_uv(samplePoint, uu, vv);
    } else {
      lower.get_triangle_uv(samplePoint, uu, vv);
    }
    *emission = mp->emitted(uu, vv, rec.p, samplePoint);
    return samplePoint;
  }

public:
  shared_ptr<material> mp;
  double x0, x1, z0, z1, k;

private:
  triangle upper;
  triangle lower;
};

inline bool xz_rect::hit(const ray &r, double t_min, double t_max,
                         record &rec) const {
  if (lower.hit(r, t_min, t_max, rec)) {
  rec.HitType = this->getType();
    return true;
  } else if (upper.hit(r, t_min, t_max, rec)) {
  rec.HitType = this->getType();
    return true;
  }
  return false;
}

class yz_rect : public hitable {
public:
  yz_rect() {}

  yz_rect(double _y0, double _y1, double _z0, double _z1, double _k,
          shared_ptr<material> mat)
      : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k),
        upper(vec3(_k, _y1, _z0), vec3(_k, _y1, _z1), vec3(_k, _y0, _z1), mat,
              true),
        lower(vec3(_k, _y1, _z0), vec3(_k, _y0, _z0), vec3(_k, _y0, _z1), mat),
        mp(mat){};

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;

  virtual bool bounding_box(AABB &output_box) const override {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    output_box = AABB(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
    return true;
  }
  vec3 getSample(record &rec, float *pdf, vec3 *emission) const override {
    vec3 samplePoint = vec3(k, rand_d(y0, y1), rand_d(z0, z1));
    float d2 = (samplePoint - rec.p).lengthSquare();
    float area = abs((y1 - y0) * (z1 - z0));
    double dd = absDot(normalize(samplePoint - rec.p), upper.normal);
    double _pdf =  area * dd/d2;
    *pdf = static_cast<float>(_pdf);
    double uu, vv;
    if (upper.is_in_tri(samplePoint)) {
      upper.get_triangle_uv(samplePoint, uu, vv);
    } else {
      lower.get_triangle_uv(samplePoint, uu, vv);
    }
    *emission = mp->emitted(uu, vv, rec.p, samplePoint);
    return samplePoint;
  }

  material::MType getMType() const override { return upper.getMType(); }

  HType_t getType() const override { return Rect; }

public:
  shared_ptr<material> mp;
  double y0, y1, z0, z1, k;

private:
  triangle upper;
  triangle lower;
};

inline bool yz_rect::hit(const ray &r, double t_min, double t_max,
                         record &rec) const {
  if (lower.hit(r, t_min, t_max, rec)) {
  rec.HitType = this->getType();
    return true;
  } else if (upper.hit(r, t_min, t_max, rec)) {
  rec.HitType = this->getType();
    return true;
  }
  return false;
}

class xy_rect : public hitable {
public:
  xy_rect() {}

  xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
          shared_ptr<material> mat)
      : y0(_y0), y1(_y1), x0(_x0), x1(_x1), k(_k),
        upper(vec3(_x0, _y1, _k), vec3(_x1, _y1, _k), vec3(_x1, _y0, _k), mat,
              true),
        lower(vec3(_x0, _y1, _k), vec3(_x0, _y0, _k), vec3(_x1, _y0, _k), mat),
        mp(mat){};

  HType_t getType() const override { return Rect; }
  material::MType getMType() const override { return upper.getMType(); }
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;

  virtual bool bounding_box(AABB &output_box) const override {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    output_box = AABB(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
    return true;
  }

  vec3 getSample(record &rec, float *pdf, vec3 *emission) const override {
    vec3 samplePoint = vec3(rand_d(x0, x1), rand_d(y0, y1), k);
    float d2 = (samplePoint - rec.p).lengthSquare();
    float area = abs((y1 - y0) * (x1 - x0));
    double dd = absDot(normalize(samplePoint - rec.p), upper.normal);
    double _pdf =  area * dd/d2;
    *pdf = static_cast<float>(_pdf);
    double uu, vv;
    if (upper.is_in_tri(samplePoint)) {
      upper.get_triangle_uv(samplePoint, uu, vv);
    } else {
      lower.get_triangle_uv(samplePoint, uu, vv);
    }
    *emission = mp->emitted(uu, vv, rec.p, samplePoint);
    return samplePoint;
  }

public:
  shared_ptr<material> mp;
  double x0, x1, y0, y1, k;

private:
  triangle upper;
  triangle lower;
};

inline bool xy_rect::hit(const ray &r, double t_min, double t_max,
                         record &rec) const {
  if (lower.hit(r, t_min, t_max, rec)) {
  rec.HitType = this->getType();
    return true;
  } else if (upper.hit(r, t_min, t_max, rec)) {
  rec.HitType = this->getType();
    return true;
  }
  return false;
}

#endif