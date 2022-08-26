#include "hitable.h"
#include "math/vector.h"
#include <algorithm>
#include <type_traits>

class AABB : hitable {
public:
  virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;
  vec3 pMin;
  vec3 pMax;
};

//To judge whether ray hits aabb by compute the overlap
inline bool AABB::hit(const ray &r, double t_min, double t_max,
                      record &rec) const {
  vec3 dir = r.direction();
  vec3 ori = r.origin();
  for (int i = 0; i < 3; i++) {
    float inv_b = 1. / dir[i];
    float t0 = (pMin[i] - ori[i]) * inv_b;
    float t1 = (pMax[i] - ori[i]) * inv_b;
    // When axis dir < 0 then swap max,min
    if (inv_b < 0.f) {
      std::swap(t0, t1);
    }
    // find the closest interval
    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t0 : t_max;
    // no overlap
    if (t_max < t_min)
      return false;
  }
  return true;
}