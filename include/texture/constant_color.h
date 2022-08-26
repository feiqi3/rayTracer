#ifndef CONSTANT_COLOR_H
#define CONSTANT_COLOR_H

#include "math/vector.h"
#include "texture/texture.h"
class constant_color : public texture {
public:
  constant_color() {}
  constant_color(const vec3 &c) : color(c) {}
  virtual vec3 value(double u, double v, const vec3 &point) const override {
    return color;
  }
  vec3 color;
};

#endif