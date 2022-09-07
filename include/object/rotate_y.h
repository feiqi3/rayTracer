#ifndef ROTATE_Y_H_
#define ROTATE_Y_H_

#include "aabb.h"
#include "tool/common.h"
class rotate_y : public hitable{
public:
        rotate_y(shared_ptr<hitable> p, double angle);

        virtual bool hit(
            const ray& r, double t_min, double t_max, record& rec) const override;

        virtual bool bounding_box(AABB& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        shared_ptr<hitable> ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        AABB bbox;
};

inline rotate_y::rotate_y(shared_ptr<hitable> p, double angle) : ptr(p) {
    auto radians = d2radian(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(bbox);

    vec3 min( Infinity,  Infinity,  Infinity);
    vec3 max(-Infinity, -Infinity, -Infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                auto newx =  cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = AABB(min, max);
}

inline bool rotate_y::hit(const ray& r, double t_min, double t_max, record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction);

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] =  cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

    normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    set_face_normal(rotated_r, normal,rec);

    return true;
}

#endif