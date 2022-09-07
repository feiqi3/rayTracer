#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "material/isotropic.h"
#include "object/aabb.h"
class constant_medium : public hitable {
    public:
        constant_medium(shared_ptr<hitable> b, double d, shared_ptr<texture> a)
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(make_shared<isotropic>(a))
            {}

        constant_medium(shared_ptr<hitable> b, double d, color c)
            : boundary(b),
              neg_inv_density(-1/d),
              phase_function(make_shared<isotropic>(c))
            {}

        virtual bool hit(
            const ray& r, double t_min, double t_max, record& rec) const override;

        virtual bool bounding_box(AABB& output_box) const override {
            return boundary->bounding_box(output_box);
        }

    public:
        shared_ptr<hitable> boundary;
        shared_ptr<material> phase_function;
        double neg_inv_density;
};

inline bool constant_medium::hit(const ray& r, double t_min, double t_max,record& rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && rand_d() < 0.00001;

    record rec1, rec2;

    if (!boundary->hit(r, -Infinity, Infinity, rec1))
        return false;
    if (!boundary->hit(r, rec1.t+0.0001, Infinity, rec2))
        return false;
    #ifdef DEBUG
    std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';
    #endif
    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(rand_d());

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  rec.t << '\n'
                  << "rec.p = " <<  rec.p << '\n';
    }

    rec.normal = vec3(1,0,0);  // arbitrary
    rec.front_face = true;     // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}
#endif