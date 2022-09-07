#ifndef ISOTROPIC_H_
#define ISOTROPIC_H_

#include "material/material.h"
#include "math/vector.h"
#include "object/hitable.h"
#include "ray.h"
#include "texture/constant_color.h"
#include "texture/texture.h"
#include <memory>
class isotropic : public material{
    public:  
    isotropic(){}
    isotropic(const std::shared_ptr<texture>& t):albedo(t){}
    isotropic(const vec3& t):albedo(make_shared<constant_color>(t)){}
    bool scatter(const ray &_in, const record &hit_rec, color &out_attenuation, ray &out_scattered) const override{
        out_scattered = ray(hit_rec.p,rand_in_unit_sphere());
        out_attenuation = albedo->value(hit_rec.u, hit_rec.v, hit_rec.p);
        return true;
    }
    std::shared_ptr<texture> albedo;
};

#endif