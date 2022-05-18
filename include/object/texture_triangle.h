/*
 * @Author: feiqi3
 * @Date: 2022-05-17 09:56:29
 * @LastEditTime: 2022-05-18 11:56:00
 * @LastEditors: feiqi3
 * @Description: |---
 Triangle with texture !
 ---|
 * @FilePath: \rayTracer\include\object\texture_triangle.h
 * ->blog: feiqi3.cn <-
 */
#ifndef TEXTURE_TRIANGLE_H
#define TEXTURE_TRIANGLE_H

#include "../buffer/RGB12Buffer.h"
#include "../material/texture.h"
#include "hitable.h"
#include "object/triangle.h"
#include <memory>

//This class must be instantiated by shared_ptr<texture_triangle>
class texture_triangle : public triangle,
                         public std::enable_shared_from_this<texture_triangle> {
public:
  texture_triangle(const vec3 &_p1, const vec3 &_p2, const vec3 &_p3,
                   const std::shared_ptr<RGB12>& _buffer);
  GET_CLASS_NAME(texture_triangle);
  virtual const std::string toString() const override {
    std::string className = clsname();
    return className + ",P1: " + p1.toString() + " ,P2: " + p2.toString() +
           " ,P3: " + p3.toString() + " ,Normal: " + normal.toString() + ".\n";
  }
  
  // This func MUST be called after ctr
  virtual void init();
  std::weak_ptr<texture> get_texture(){return _tex;}
protected:
  // to avoid cycle reference
  std::shared_ptr<texture> _tex;
};
inline texture_triangle::texture_triangle(const vec3 &_p1, const vec3 &_p2,
                                          const vec3 &_p3, const std::shared_ptr<RGB12>& _buffer)
    : triangle(_p1, _p2, _p3) {
  _tex = std::make_shared<texture>(_buffer);
  triangle::mat_ptr = _tex;
  // member function in enable_shared_from_this<T>
  Flog::flog(INFO, toString());
}

inline void texture_triangle::init() {
  std::weak_ptr<texture_triangle> _this_ptr = shared_from_this();
   _tex->set_triangle(_this_ptr.lock());
}
#endif