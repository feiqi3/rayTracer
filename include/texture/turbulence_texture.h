#ifndef TURB_TEXTURE_H
#define TURB_TEXTURE_H

#include "math/vector.h"
#include "texture/noise_texture.h"
#include "texture/texture.h"
class turb_texture :public noise_texture {
public:
  turb_texture() {}
  turb_texture(double _scale) : noise_texture(_scale) {}
  virtual vec3 value(double u, double v, const vec3 &point) const override;
};
inline vec3 turb_texture::value(double u, double v, const vec3 &point)const{
    return vec3(noise.turb_noise(scale * point));
}
#endif // TURB_TEXTURE_H