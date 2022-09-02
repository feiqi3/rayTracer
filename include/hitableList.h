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

#include "object/aabb.h"
#include "object/hitable.h"
#include <memory>
#include <vector>

class hitable_list :public hitable {
public:
  std::vector<std::shared_ptr<object>> obj_list;

public:
  hitable_list() {}

  hitable_list(std::shared_ptr<object> obj) { add(obj); }

  void add(const std::shared_ptr<object>& obj) { obj_list.push_back(obj); }

  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;
  bool bounding_box(AABB &box_out) const override;
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

inline bool hitable_list::bounding_box(AABB &box_out)const{
  if(obj_list.empty()){
    return false;
  }
  AABB tmp_Box;
  bool isFirstBox = true;
  for (const auto& i: obj_list) {
    if(!i->bounding_box(tmp_Box))return false;
    box_out = isFirstBox ? tmp_Box : AABB::surrounding_box(tmp_Box, box_out);
    isFirstBox = false;
  }
  return true;
}
#endif