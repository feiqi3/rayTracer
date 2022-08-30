#ifndef MARBLE_H
#define MARBLE_H
#include "texture/noise_texture.h"
#include <cmath>

class marble_texture : public noise_texture {
public:
  marble_texture() {}
  marble_texture(double _scale) : noise_texture(_scale) {}
  vec3 value(double u, double v, const vec3 &point) const {
    return 0.5 * (1 + sin(scale * point.z() + 10 * noise.turb_noise(point)));
  }
};

#endif