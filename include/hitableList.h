/*
 * @Author: feiqi3
 * @Date: 2022-02-02 11:51:26
 * @LastEditTime: 2022-05-18 13:15:46
 * @LastEditors: feiqi3
 * @Description: |a list of objects|
 * @FilePath: \rayTracer\include\hitableList.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _HITABLELIST_H_
#define _HITABLELIST_H_

#include "Macro.h"
#include "object/hitable.h"
#include <memory>
#include <string>
#include <vector>
class hitable_list : hitable {
public:
//object == hitable
  std::vector<std::shared_ptr<object>> obj_list;

public:
  hitable_list() {}

  hitable_list(std::shared_ptr<object> obj) { add(obj); }

  GET_CLASS_NAME(hitableList)
  const std::string toString() const override
  {
    std::string classname = clsname();
    return classname+","+ STR(obj_list.size())+" object in map.";
  }

  void add(const std::shared_ptr<object>& obj) { obj_list.push_back(obj); }

  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override {

    bool is_hit = false;
    double t_closest = t_max;
    record temp_rec;
    for (const auto &obj : obj_list) {
      if (obj->hit(r, t_min, t_closest, temp_rec)) {
        is_hit = true;
        t_closest = temp_rec.t;
        rec = temp_rec;
      }
    }
    return is_hit;
  }
};

#endif