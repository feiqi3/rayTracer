#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H
#include "math/vector.h"
#include "texture.h"
#include "../noise/perlin_noise_3d.h"
class noise_texture :public texture{
public:
  noise_texture() {}
  
  //use scale to control frequent
  noise_texture(double _scale):scale(_scale){};
  virtual vec3 value(double u, double v, const vec3 &point) const override;
protected:
    perlin3D noise;
    double scale = 1;
};

vec3 noise_texture::value(double u, double v, const vec3 &point)const{
    return vec3(noise.noise01(point*scale));
}

#endif // NOISE_TEXTURE_H