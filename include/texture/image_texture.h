#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "texture/texture.h"
#include "tool/picTool.h"
class image_texture : public texture {
public:
  image_texture() {}
  image_texture(const char *filename, bool filter = false);
  vec3 value(double u, double v, const vec3 &point) const override;

private:
  bool linearInterpolation = true;
  std::shared_ptr<RGB12> image_buffer;
};

inline image_texture::image_texture(const char *filename, bool filter)
    : image_buffer(fPic::get_buffer_from_pic(filename)) {
  image_buffer->setInterpolation(filter);
}

inline vec3 image_texture::value(double u, double v, const vec3 &point) const {
  u = clamp(u, 0.0, 1.0);
  v = clamp(v, 0.0, 1.0);

  return image_buffer->sampler(u, v);
}

#endif // IMAGE_TEXTURE_H