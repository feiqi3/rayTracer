#ifndef _AABB_H_
#define _AABB_H_
#include "hitable.h"
#include "math/vector.h"
#include <algorithm>
#include <type_traits>

class AABB :public hitable {
public:
  AABB(){}
  AABB(const vec3 &pmin, const vec3 &pmax) : pMin(pmin), pMax(pmax){};
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;
  vec3 min() const;
  vec3 max() const;
  virtual bool bounding_box(AABB &box_out) const override;
  static AABB surrounding_box(AABB &box_1, AABB &box_2);

private:
  vec3 pMin;
  vec3 pMax;
};

// To judge whether ray hits aabb by compute the overlap
inline bool AABB::hit(const ray &r, double t_min, double t_max,
                      record &rec) const {
  vec3 dir = r.direction();
  vec3 ori = r.origin();
  for (int i = 0; i < 3; i++) {
    float inv_b = 1. / dir[i];
    float t0 = (min()[i] - ori[i]) * inv_b;
    float t1 = (max()[i] - ori[i]) * inv_b;
    // When axis dir < 0 then swap max,min
    if (inv_b < 0.f) {
      std::swap(t0, t1);
    }
    // find the closest interval
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;
    // no overlap
    if (t_max < t_min)
      return false;
  }
  return true;
}
inline bool AABB::bounding_box(AABB &box_out) const {
  box_out = *this;
  return true;
}

inline AABB AABB::surrounding_box(AABB &box_1, AABB &box_2) {
  vec3 small(fmin(box_1.min().x(), box_2.min().x()),
             fmin(box_1.min().y(), box_2.min().y()),
             fmin(box_1.min().z(), box_2.min().z()));
  vec3 big(fmax(box_1.max().x(), box_2.max().x()),
           fmax(box_1.max().y(), box_2.max().y()),
           fmax(box_1.max().z(), box_2.max().z()));
           return AABB(small,big);
}

inline vec3 AABB::min() const { return pMin; }
inline vec3 AABB::max() const { return pMax; }

#endif // AABB_H_