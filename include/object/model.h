/*
 * @Author: feiqi3
 * @Date: 2022-05-21 15:12:53
 * @LastEditTime: 2022-05-24 11:54:47
 * @LastEditors: feiqi3
 * @Description: |---BUGS here---|
 * @FilePath: \rayTracer\include\object\model.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _MODEL_H_
#define _MODEL_H_

#include "hitable.h"
#include "texture_triangle.h"
#include <limits>
#include <string>
#include <vector>
#include <winuser.h>
class model : public hitable {
public:
  model() {center_p = vec3(0,0,0);}
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;
  void add(const std::shared_ptr<texture_triangle> &);
  GET_CLASS_NAME(Model);
  virtual const std::string toString() const override;

  void transform(const mat4&);
protected:
  std::vector<std::shared_ptr<texture_triangle>> t_list;

};

inline bool model::hit(const ray &r, double t_min, double t_max,
                       record &rec) const {
  float __t = std::numeric_limits<float>::infinity();
  float hit_flag = false;
  record tmp;
  tmp.t = -1;
  for (auto i : t_list) {
    if (i->hit(r, t_min, t_max, rec)) {
      hit_flag = true;
      if (rec.t < __t && rec.t > t_min) {
        __t = rec.t;
        tmp = rec;
      }
    }
  }
  if (hit_flag) {
    rec = tmp;
  }
  return hit_flag;
}

inline const std::string model::toString() const { return "Model made\n"; }

inline void model::add(const std::shared_ptr<texture_triangle> &t) {
  t_list.push_back(t);
}

inline void model::transform(const mat4 &mat) {
  for (auto i : t_list) {
    i->transform(mat);
  }
  center_p = (mat * vec4(center_p,1)).xyz();
}

#endif