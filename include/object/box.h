#ifndef BOX_H
#define BOX_H

#include "aarect.h"
#include "hitableList.h"
#include "object/aabb.h"
#include "object/bvh_node.h"
#include "object/hitable.h"
#include "tool/common.h"

class box : public hitable_list {
public:
  box() {}
  box(const vec3 &p0, const vec3 &p1, shared_ptr<material> ptr);

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;

  virtual bool bounding_box(AABB &output_box) const override {
    bvh.bounding_box(output_box);
    return true;
  }
  vec3 getSample(record &rec, float *pdf,vec3*emission) const override;

public:
  vec3 box_min;
  vec3 box_max;

  bvh_node bvh;

  material::MType getMType() const override {
    return hitable_list::obj_list[0]->getMType();
  }
  HType_t getType() const override { return Htype::Set|Htype::Box; }
};

inline vec3 box::getSample(record &rec, float *pdf,vec3*emission) const {
  float d = rand_d();
  return hitable_list::obj_list[int(d * 6)]->getSample(rec, pdf,emission);
}

inline box::box(const vec3 &p0, const vec3 &p1, shared_ptr<material> ptr) {
  box_min = p0;
  box_max = p1;
  hitable_list &sides = *this;
  sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
  sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

  sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
  sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

  sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
  sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
  bvh = bvh_node(sides);
}

inline bool box::hit(const ray &r, double t_min, double t_max,
                     record &rec) const {
  if(bvh.hit(r, t_min, t_max, rec)){
  rec.HitType = this->getType();
    return true;
  }
  return false;
}

#endif