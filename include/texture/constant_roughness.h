#ifndef CONSTANT_ROUGHNESS_H
#define CONSTANT_ROUGHNESS_H

#include "math/vector.h"
#include "texture/constant_color.h"
#include "texture/texture.h"


class constant_roughness : public texture {
public:
  constant_roughness(float _roughnessX,float _roughnessY):roughness(_roughnessX,_roughnessY,0) {}
  constant_roughness(float _roughness) : roughness(_roughness){}
  vec3 value(double u, double v, const vec3 &point) const override{
    return roughness;
  }
  vec3 roughness;
};
typedef constant_roughness cRough;
#endif