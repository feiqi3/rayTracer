/*
 * @Author: feiqi3
 * @Date: 2022-02-02 11:51:26
 * @LastEditTime: 2022-05-24 12:19:31
 * @LastEditors: feiqi3
 * @Description: |a list of objects|
 * @FilePath: \rayTracer\include\hitableList.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _HITABLELIST_H_
#define _HITABLELIST_H_

#include "Macro.h"
#include "material/material.h"
#include "math/vector.h"
#include "object/hitable.h"
#include <memory>
#include <string>
#include <vector>
class hitable_list : hitable {
public:
  // object == hitable
  std::vector<std::shared_ptr<object>> obj_list;

public:
  hitable_list() {}

  hitable_list(std::shared_ptr<object> obj) { add(obj); }

  GET_CLASS_NAME(hitableList)
  const std::string toString() const override {
    std::string classname = clsname();
    return classname + "," + STR(obj_list.size()) + " object in map.";
  }

  static std::vector<shared_ptr<object>> light_list;

  void addLight(const shared_ptr<object> &obj,
                color clr) {
    obj->mat_ptr->setLightandColor(clr);
    light_list.push_back(std::ref(obj));
    obj_list.push_back(obj);
    LightColor += clr;
  }
  void add(const std::shared_ptr<object> &obj) { obj_list.push_back(obj); }

  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override {

    bool is_hit = false;
    double t_closest = t_max;
    record temp_rec;
    record __record;
    for (const auto &obj : obj_list) {
      if (obj->hit(r, t_min, t_closest, temp_rec)) {
        is_hit = true;
        if (temp_rec.t < t_closest) {
          t_closest = temp_rec.t;
          __record = temp_rec;
        }
      }
    }
    rec = __record;
    return is_hit;
  }
static const vec3 getLightColor()
{
  if(light_list.size() == 0)return vec3(0,0,0);
  return LightColor / light_list.size();  
} 
protected:
  static vec3 LightColor;
};

vec3 hitable_list::LightColor = vec3(0,0,0);
std::vector<std::shared_ptr<object>> hitable_list::light_list;
#endif