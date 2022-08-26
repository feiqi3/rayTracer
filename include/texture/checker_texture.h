#ifndef CHECKER_TEXTURE_H
#define CHECKER_TEXTURE_H
#include "math/vector.h"
#include "texture.h"
#include <cmath>
#include <memory>

class checker_texture : public texture {
public:
  checker_texture(){};
  checker_texture(const std::shared_ptr<texture> &_even,
                  const std::shared_ptr<texture> &_odd);
  vec3 value(double u, double v, const vec3 &point) const override;
  shared_ptr<texture> even;
  shared_ptr<texture> odd;
};

inline checker_texture::checker_texture(const std::shared_ptr<texture> &_even,
                                        const std::shared_ptr<texture> &_odd)
    : even(_even), odd(_odd) {}
inline vec3 checker_texture::value(double u, double v,
                                   const vec3 &point) const {
  double sine = sin(10 * point.x()) * sin(10 * point.y()) * sin(10 * point.z());
  if (sine > 0)
    return even->value(u, v, point);
  else
    return odd->value(u, v, point);
}
#endif