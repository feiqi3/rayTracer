#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include "object/hitable.h"
#include "object/triangle.h"
#include "ray.h"
#include <memory>
class translate : public hitable {
public:
  translate(std::shared_ptr<hitable> &p, const vec3 &displacement)
      : tar(p), offset(displacement) {}
  bool bounding_box(AABB &box_out) const override;
  bool hit(const ray &r, double t_min, double t_max,
           record &rec) const override;

private:
  std::shared_ptr<hitable> tar;
  vec3 offset;
};

inline bool translate::hit(const ray &r, double t_min, double t_max,
                           record &rec) const {
  ray ray_moved(r.origin() - offset, r.dir);
  if (!tar->hit(ray_moved, t_min, t_max, rec)) {
    return false;
  }
  rec.p = rec.p + offset;
  set_face_normal(ray_moved, rec.normal, rec);
  return true;
}

inline bool translate::bounding_box(AABB &box_out) const {
  if (!tar->bounding_box(box_out))
    return false;
  box_out = AABB(box_out.min() + offset, box_out.max() + offset);
  return true;
}
#endif // TANSLATION_H_