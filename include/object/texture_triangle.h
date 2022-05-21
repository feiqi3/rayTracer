/*
 * @Author: feiqi3
 * @Date: 2022-05-17 09:56:29
 * @LastEditTime: 2022-05-21 16:02:05
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

// This class must be instantiated by shared_ptr<texture_triangle>
class texture_triangle : public triangle,
                         public std::enable_shared_from_this<texture_triangle> {
public:
  texture_triangle(const vec3 &_p1, const vec3 &_p2, const vec3 &_p3,
                   const std::shared_ptr<RGB12> &_buffer, shared_ptr<material>);
  GET_CLASS_NAME(texture_triangle);
  virtual const std::string toString() const override {
    std::string className = clsname();
    return className + ",P1: " + p1.toString() + " ,P2: " + p2.toString() +
           " ,P3: " + p3.toString() + " ,Normal: " + normal.toString() + ".\n";
  }
  bool has_init() { return is_init; }
  // This func MUST be called after ctr
  virtual void init();
  texture_triangle(const vec3 &_p1, const vec3 &_p2, const vec3 &_p3,
                   const vec3 &tc1, const vec3 &tc2, const vec3 &tc3,
                   const std::shared_ptr<RGB12> &_buffer,
                   shared_ptr<material> _mat);
  // Useless
  std::weak_ptr<texture> get_texture() { 
      return _tex; }

protected:
  bool is_init;
  std::shared_ptr<texture> _tex;
  //  std::shared_ptr<material> _mat;
};
inline texture_triangle::texture_triangle(
    const vec3 &_p1, const vec3 &_p2, const vec3 &_p3,
    const std::shared_ptr<RGB12> &_buffer,
    shared_ptr<material> _mat = std::make_shared<lambertian>(vec3(1, 1, 1)))
    : triangle(_p1, _p2, _p3) {
  is_init = false;
  _tex = std::make_shared<texture>(_buffer, _mat);
  triangle::mat_ptr = _tex;
  // member function in enable_shared_from_this<T>
  Flog::flog(INFO, toString());
}

//这个材质它里面必须要有一个指向这个三角形的指针
//但是这个指针只有在这个类被智能指针初始化后才能使用
//所以一定得在构造函数执行后调用init()
inline void texture_triangle::init() {
  std::weak_ptr<texture_triangle> _this_ptr = shared_from_this();
  _tex->set_triangle(_this_ptr.lock());
  is_init = true;
}

inline texture_triangle::texture_triangle(
    const vec3 &_p1, const vec3 &_p2, const vec3 &_p3, const vec3 &tc1,
    const vec3 &tc2, const vec3 &tc3, const std::shared_ptr<RGB12> &_buffer,
    shared_ptr<material> _mat = std::make_shared<lambertian>(vec3(1, 1, 1)))
    : triangle(_p1, _p2, _p3) {
  is_init = false;
  _tex = std::make_shared<texture>(_buffer, _mat);
  triangle::mat_ptr = _tex;
  set_texcoord(tc1, tc2, tc3);
}
#endif