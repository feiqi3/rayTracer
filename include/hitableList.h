/*
 * @Author: feiqi3
 * @Date: 2022-02-02 11:51:26
 * @LastEditTime: 2022-03-04 00:05:49
 * @LastEditors: feiqi3
 * @Description: |a list of objects|
 * @FilePath: \rayTracer\include\hitableList.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _HITABLELIST_H_
#define _HITABLELIST_H_

#include "material/material.h"
#include "object/aabb.h"

#include "object/hitable.h"
#include "object/translate.h"
#include <memory>
#include <vector>

class bvh_node;

class hitable_list : public hitable {
public:
  std::vector<std::shared_ptr<object>> obj_list;

public:
  hitable_list() {}
  virtual ~hitable_list() {}
  hitable_list(std::shared_ptr<object> obj) {}

  virtual void add(const std::shared_ptr<object> &obj) {
    obj_list.push_back(obj);
    shared_ptr<object> tmp_obj = obj;
    //Get the REAL object
    //如果这个物体被transform包裹着
    //那么就难以知道其是否为盒子这样的物体
    //于是取出其真正的内容，方便判断
    while (tmp_obj->getType() & Wrapper) {
      tmp_obj = std::dynamic_pointer_cast<transform>(tmp_obj)->get();
    }
    if (tmp_obj->getMType() == material::Light) {
      if (tmp_obj->getType() & Set) {
        //If the objects (Like box) is consisted of many basic primitives (like rectangle)
        //Then add it to the vector Lights as the number of its primitives
        //如果这个物体由多个小的物体组成（比如盒子是由轴对齐矩形组成)
        //那么就加入和其包含物体同样多数量的自身副本  
        //因为对于盒子光源，是随机选一个其中的物体进行采样
        //或者像模型这样多面的光源. 当然，不推荐用模型当光源。
        hitable_list *tmp_ptr = dynamic_cast<hitable_list *>(tmp_obj.get());
        for (int i = 0; i < tmp_ptr->obj_list.size(); ++i) {
          Lights.push_back(obj);
        }
        return;
      }
      Lights.push_back(obj);
    }
  }

  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;
  virtual bool bounding_box(AABB &box_out) const override;
  virtual vec3 getSample(record &rec, float *pdf,vec3* emission) const override {
    return hitable::getSample(rec, pdf,emission);
  }
  material::MType getMType() const override{
    return material::Other;
  }
  friend class bvh_node;

protected:
  std::vector<std::shared_ptr<object>> Lights;
};

inline bool hitable_list::hit(const ray &r, double t_min, double t_max,
                              record &rec) const {
  bool is_hit = false;
  double t_closest = t_max;
  record temp_rec;
  for (const auto &obj : obj_list) {
    if (obj->hit(r, t_min, t_closest, temp_rec)) {
      is_hit = true;
      // Find the closest target
      if (temp_rec.t < t_closest) {
        t_closest = temp_rec.t;
        rec = temp_rec;
      }
    }
  }
  return is_hit;
}

inline bool hitable_list::bounding_box(AABB &box_out) const {
  if (obj_list.empty()) {
    return false;
  }
  AABB tmp_Box;
  bool isFirstBox = true;
  for (const auto &i : obj_list) {
    if (!i->bounding_box(tmp_Box))
      return false;
    box_out = isFirstBox ? tmp_Box : AABB::surrounding_box(tmp_Box, box_out);
    isFirstBox = false;
  }
  return true;
}
#endif