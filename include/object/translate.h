#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include "math/vector.h"
#include "object/hitable.h"
#include "object/triangle.h"
#include "ray.h"
#include <memory>

class transform : public hitable {
public:
  virtual const shared_ptr<object> &get() const = 0;
  virtual ~transform() {}
  virtual HType_t getType() const override{return Wrapper|get()->getType();}
  virtual material::MType getMType() const override{
    return get()->getMType();
  }
};

class translate : public transform {
public:
  translate(std::shared_ptr<hitable> &p, const vec3 &displacement)
      : tar(p), offset(displacement) {}
  bool bounding_box(AABB &box_out) const override;
  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;
  vec3 getSample(record &rec, float *pdf,vec3* emission) const override;
  const shared_ptr<object> & get() const override{return tar;}
private:
  std::shared_ptr<hitable> tar;
  vec3 offset;
};

inline vec3 translate::getSample(record &rec, float *pdf,vec3* emission) const {
  record newRecord = rec;
  newRecord.p = newRecord.p - offset;
  return tar->getSample(newRecord, pdf,emission) + offset;
}

inline bool translate::hit(const ray &r, double t_min, double t_max,
                           record &rec) const {
  ray ray_moved(r.origin() - offset, r.dir);
  if (!tar->hit(ray_moved, t_min, t_max, rec)) {
    return false;
  }
  rec.p = rec.p + offset;
  set_face_normal(ray_moved, rec.normal, rec);
  rec.HitType = this->getType();
  return true;
}

inline bool translate::bounding_box(AABB &box_out) const {
  if (!tar->bounding_box(box_out))
    return false;
  box_out = AABB(box_out.min() + offset, box_out.max() + offset);
  return true;
}

class rotate_y : public transform {
public:
  rotate_y(shared_ptr<hitable> p, double angle);

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;

  virtual bool bounding_box(AABB &output_box) const override {
    output_box = bbox;
    return hasbox;
  }
  vec3 getSample(record &rec, float *pdf,vec3* emission) const override;
  const shared_ptr<object> & get() const override{return ptr;}
public:
  shared_ptr<hitable> ptr;
  double sin_theta;
  double cos_theta;
  bool hasbox;
  AABB bbox;
};

inline rotate_y::rotate_y(shared_ptr<hitable> p, double angle) : ptr(p) {
  auto radians = d2radian(angle);
  sin_theta = sin(radians);
  cos_theta = cos(radians);
  hasbox = ptr->bounding_box(bbox);

  vec3 min(Infinity, Infinity, Infinity);
  vec3 max(-Infinity, -Infinity, -Infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
        auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
        auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

        auto newx = cos_theta * x + sin_theta * z;
        auto newz = -sin_theta * x + cos_theta * z;

        vec3 tester(newx, y, newz);

        for (int c = 0; c < 3; c++) {
          min[c] = fmin(min[c], tester[c]);
          max[c] = fmax(max[c], tester[c]);
        }
      }
    }
  }

  bbox = AABB(min, max);
}

inline bool rotate_y::hit(const ray &r, double t_min, double t_max,
                          record &rec) const {
  auto origin = r.origin();
  auto direction = r.direction();

  origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
  origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

  direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
  direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

  ray rotated_r(origin, direction);

  if (!ptr->hit(rotated_r, t_min, t_max, rec))
    return false;

  auto p = rec.p;
  auto normal = rec.normal;

  p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
  p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
  // Because the rotate matrix is an orthogonal matrix.
  normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
  normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

  rec.p = p;
  rec.normal = normal;
  set_face_normal(rotated_r, normal, rec);
  rec.HitType = this->getType();
  return true;
}

inline vec3 rotate_y::getSample(record &rec, float *pdf,vec3* emission) const {
  record newRecord = rec;
  auto &pp = newRecord.p;
  pp[0] = cos_theta * pp[0] - sin_theta * pp[2];
  pp[2] = sin_theta * pp[0] + cos_theta * pp[2];

  auto &nn = newRecord.normal;
  nn[0] = cos_theta * nn[0] - sin_theta * nn[2];
  nn[2] = sin_theta * nn[0] + cos_theta * nn[2];

  vec3 res = ptr->getSample(newRecord, pdf,emission);
  res[0] = cos_theta * res[0] + sin_theta * res[2];
  res[2] = -sin_theta * res[0] + cos_theta * res[2];
  return res;
}

class rotate_x : public transform {
public:
  rotate_x(shared_ptr<hitable> p, double angle);

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;

  virtual bool bounding_box(AABB &output_box) const override {
    output_box = bbox;
    return hasbox;
  }

  vec3 getSample(record &rec, float *pdf,vec3* emission) const override;

  const shared_ptr<object> & get() const override{return ptr;}
public:
  shared_ptr<hitable> ptr;
  double sin_theta;
  double cos_theta;
  bool hasbox;
  AABB bbox;
};

inline rotate_x::rotate_x(shared_ptr<hitable> p, double angle) : ptr(p) {
  auto radians = d2radian(angle);
  sin_theta = sin(radians);
  cos_theta = cos(radians);
  hasbox = ptr->bounding_box(bbox);

  vec3 min(Infinity, Infinity, Infinity);
  vec3 max(-Infinity, -Infinity, -Infinity);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
        auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
        auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

        auto newy = cos_theta * y - sin_theta * z;
        auto newz = sin_theta * y + cos_theta * z;

        vec3 tester(x, newy, newz);

        for (int c = 0; c < 3; c++) {
          min[c] = fmin(min[c], tester[c]);
          max[c] = fmax(max[c], tester[c]);
        }
      }
    }
  }

  bbox = AABB(min, max);
}

inline bool rotate_x::hit(const ray &r, double t_min, double t_max,
                          record &rec) const {
  auto origin = r.origin();
  auto direction = r.direction();

  origin[1] = cos_theta * r.origin()[1] + sin_theta * r.origin()[2];
  origin[2] = -sin_theta * r.origin()[1] + cos_theta * r.origin()[2];

  direction[1] = cos_theta * r.direction()[1] + sin_theta * r.direction()[2];
  direction[2] = -sin_theta * r.direction()[1] + cos_theta * r.direction()[2];

  ray rotated_r(origin, direction);

  if (!ptr->hit(rotated_r, t_min, t_max, rec))
    return false;

  auto p = rec.p;
  auto normal = rec.normal;

  p[1] = cos_theta * rec.p[1] - sin_theta * rec.p[2];
  p[2] = sin_theta * rec.p[1] + cos_theta * rec.p[2];

  normal[1] = cos_theta * rec.normal[1] - sin_theta * rec.normal[2];
  normal[2] = sin_theta * rec.normal[1] + cos_theta * rec.normal[2];

  rec.p = p;
  rec.normal = normal;
  set_face_normal(rotated_r, normal, rec);
  rec.HitType = this->getType();
  return true;
}

inline vec3 rotate_x::getSample(record &rec, float *pdf,vec3* emission) const {
  record newRecord = rec;
  auto &pp = newRecord.p;
  pp[1] = cos_theta * pp[1] + sin_theta * pp[2];
  pp[2] = -sin_theta * pp[1] + cos_theta * pp[2];

  auto &nn = newRecord.normal;
  nn[1] = cos_theta * nn[1] + sin_theta * nn[2];
  nn[2] = -sin_theta * nn[1] + cos_theta * nn[2];

  vec3 res = ptr->getSample(newRecord, pdf,emission);
  res[1] = cos_theta * res[1] - sin_theta * res[2];
  res[2] = sin_theta * res[1] + cos_theta * res[2];
  return res;
}

#endif // TANSLATION_H_