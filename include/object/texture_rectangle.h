/*
 * @Author: feiqi3
 * @Date: 2022-05-17 09:56:29
 * @LastEditTime: 2022-05-21 10:10:42
 * @LastEditors: feiqi3
 * @Description: |---rectangle with texture!---|
 * @FilePath: \rayTracer\include\object\texture_rectangle.h
 * ->blog: feiqi3.cn <-
 */
#ifndef TEXTURE_RECTANGLE_H
#define TEXTURE_RECTANGLE_H

#include "../buffer/RGB12Buffer.h"
#include "../material/texture.h"
#include "hitable.h"
#include "math/vector.h"
#include "object/triangle.h"
#include "texture_triangle.h"
#include <memory>

class texture_rectangle : public texture_triangle {
public:
  texture_rectangle(const vec3 &lower_left, const vec3 &upper_left,
                    const vec3 &upper_right, const vec3 &lower_right,
                    const std::shared_ptr<RGB12> &_buf,shared_ptr<material>);
  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;
  virtual barycoord get_barycentric(const vec3 &point,
                                    const record &hit_rec) override;
  virtual void transform(const mat4 &) override;
  virtual void init() override;

protected:
  shared_ptr<texture_triangle> tri;
};

inline texture_rectangle::texture_rectangle(const vec3 &lower_left,
                                            const vec3 &upper_left,
                                            const vec3 &upper_right,
                                            const vec3 &lower_right,
                                            const std::shared_ptr<RGB12> &_buf,shared_ptr<material> _mat= std::make_shared<lambertian>(vec3(1,1,1)))
    : texture_triangle(upper_left, lower_left, lower_right, _buf,_mat) {
  texture_triangle::set_texcoord(vec3(0, 1, 0), vec3(0, 0, 0), vec3(1, 0, 0));
  tri = std::make_shared<texture_triangle>(upper_left, upper_right, lower_right,
                                           _buf,_mat);
  tri->set_texcoord(vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 0, 0));
}

inline bool texture_rectangle::hit(const ray &r, double t_min, double t_max,
                                   record &rec) const {
  if (texture_triangle::hit(r, t_min, t_max, rec) ||
      tri->hit(r, t_min, t_max, rec)) {
    return true;
  } else {
    return false;
  }
}
inline void texture_rectangle::init() {
  texture_triangle::init();
  tri->init();
}
inline barycoord texture_rectangle::get_barycentric(const vec3 &point,
                                                    const record &hit_rec) {
  if (hit_rec.mat_ptr == mat_ptr) {
    return texture_triangle::get_barycentric(point, hit_rec);
  } else {
    return tri->get_barycentric(point, hit_rec);
  }
}
inline void texture_rectangle::transform(const mat4 &mat) {
  triangle::transform(mat);
  tri->transform(mat);
}
#endif