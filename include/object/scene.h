/*
 * @Author: feiqi3
 * @Date: 2022-05-22 21:15:30
 * @LastEditTime: 2022-05-23 09:55:10
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\object\scene.h
 * ->blog: feiqi3.cn <-
 */
#ifndef SCENE_H
#define SCENE_H

#include "object/hitable.h"
#include "texture_rectangle.h"
#include <limits>
#include <memory>
class scene : public texture_rectangle {
public:
  scene(const vec3 &mid_p, float r, const char *path)
      : texture_rectangle(), center(mid_p) {
    std::string baseP = std::string(path) + "sky512_";
    shared_ptr<RGB12> _down =
        std::make_shared<RGB12>((baseP + "dn.tex").c_str());
    shared_ptr<RGB12> _up = std::make_shared<RGB12>((baseP + "up.tex").c_str());
    shared_ptr<RGB12> _right =
        std::make_shared<RGB12>((baseP + "lf.tex").c_str());
    shared_ptr<RGB12> _left =
        std::make_shared<RGB12>((baseP + "rt.tex").c_str());
    shared_ptr<RGB12> _back =
        std::make_shared<RGB12>((baseP + "bk.tex").c_str());
    shared_ptr<RGB12> _forward =
        std::make_shared<RGB12>((baseP + "ft.tex").c_str());
    auto base_tex = std::make_shared<lambertian>(vec3(1, 1, 1));
    base_tex->setLightandColor(true, vec3(1, 1, 1));
    top = std::make_shared<texture_rectangle>(
        vec3(mid_p[0] + r, mid_p[1] + r, mid_p[2] - r),
        vec3(mid_p[0] - r, mid_p[1] + r, mid_p[2] - r),
        vec3(mid_p[0] - r, mid_p[1] + r, mid_p[2] + r),
        vec3(mid_p[0] + r, mid_p[1] + r, mid_p[2] + r), _up, base_tex, true);
    top->init();
    right = std::make_shared<texture_rectangle>(
        vec3(mid_p[0] + r, mid_p[1] - r, mid_p[2] - r),
        vec3(mid_p[0] + r, mid_p[1] + r, mid_p[2] - r),
        vec3(mid_p[0] + r, mid_p[1] + r, mid_p[2] + r),
        vec3(mid_p[0] + r, mid_p[1] - r, mid_p[2] + r), _right, base_tex, true);
    right->init();
    left = std::make_shared<texture_rectangle>(
        vec3(mid_p[0] - r, mid_p[1] - r, mid_p[2] + r),
        vec3(mid_p[0] - r, mid_p[1] + r, mid_p[2] + r),
        vec3(mid_p[0] - r, mid_p[1] + r, mid_p[2] - r),
        vec3(mid_p[0] - r, mid_p[1] - r, mid_p[2] - r), _left, base_tex, true);
    left->init();
    forward = std::make_shared<texture_rectangle>(
        vec3(mid_p[0] - r, mid_p[1] - r, mid_p[2] - r),
        vec3(mid_p[0] - r, mid_p[1] + r, mid_p[2] - r),
        vec3(mid_p[0] + r, mid_p[1] + r, mid_p[2] - r),
        vec3(mid_p[0] + r, mid_p[1] - r, mid_p[2] - r), _forward, base_tex,
        true);
    forward->init();
    back = std::make_shared<texture_rectangle>(
        vec3(mid_p[0] + r, mid_p[1] - r, mid_p[2] + r),
        vec3(mid_p[0] + r, mid_p[1] + r, mid_p[2] + r),
        vec3(mid_p[0] - r, mid_p[1] + r, mid_p[2] + r),
        vec3(mid_p[0] - r, mid_p[1] - r, mid_p[2] + r), _back, base_tex, true);
    back->init();
    down = std::make_shared<texture_rectangle>(
        vec3(mid_p[0] - r, mid_p[1] - r, mid_p[2] + r),
        vec3(mid_p[0] - r, mid_p[1] - r, mid_p[2] - r),
        vec3(mid_p[0] + r, mid_p[1] - r, mid_p[2] - r),
        vec3(mid_p[0] + r, mid_p[1] - r, mid_p[2] + r), _down, base_tex, true);
    down->init();
  }
  const std::string toString() const override {
    return "Sky box generated. Middle in" + center.toString();
  }

  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override {
    double __t = std::numeric_limits<float>::infinity();
    float hit_flag = false;
    record tmp;

    /*
    Ooooh no! How could it be so ugly
    But i dont want to fix it :)
    */

    if (forward->hit(r, t_min, t_max, rec)) {
      if (rec.t < __t) {
        __t = rec.t;
        hit_flag = true;
        tmp = rec;
      }
    }
    if (top->hit(r, t_min, t_max, rec)) {
      if (rec.t < __t) {
        __t = rec.t;
        hit_flag = true;
        tmp = rec;
      }
    }
    if (back->hit(r, t_min, t_max, rec)) {
      if (rec.t < __t) {
        __t = rec.t;
        hit_flag = true;
        tmp = rec;
      }
    }
    if (down->hit(r, t_min, t_max, rec)) {
      if (rec.t < __t) {
        __t = rec.t;
        hit_flag = true;
        tmp = rec;
      }
    }
    if (left->hit(r, t_min, t_max, rec)) {
      if (rec.t < __t) {
        __t = rec.t;
        hit_flag = true;
        tmp = rec;
      }
    }
    if (right->hit(r, t_min, t_max, rec)) {
      if (rec.t < __t) {
        __t = rec.t;
        hit_flag = true;
        tmp = rec;
      }
    }
    rec = tmp;
    return hit_flag;
  }

protected:
  shared_ptr<texture_rectangle> down;
  shared_ptr<texture_rectangle> right;
  shared_ptr<texture_rectangle> left;
  shared_ptr<texture_rectangle> forward;
  shared_ptr<texture_rectangle> back;
  shared_ptr<texture_rectangle> top;
  vec3 center;

private:
};
#endif
