/*
 * @Author: feiqi3
 * @Date: 2022-05-21 15:12:53
 * @LastEditTime: 2022-05-21 16:21:22
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\object\model.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _MODEL_H_
#define _MODEL_H_

#include "hitable.h"
#include "texture_triangle.h"
#include <string>
#include <vector>
#include <winuser.h>
class model : public hitable {
public:
  model() {}
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;
  void add(const texture_triangle &);
  GET_CLASS_NAME(Model);
  virtual const std::string toString() const override;
protected:
  std::vector<texture_triangle> t_list;
};

inline bool model::hit(const ray &r, double t_min, double t_max, record &rec) const 
{
    for(auto i : t_list)
    {
       if(i.hit(r, t_min,t_max,rec))return true;
    }
    return false;
}

inline const std::string model::toString()const 
{
  return "Model made\n";
}

inline void model::add(const texture_triangle & t)
{
    t_list.push_back(t);
}
#endif