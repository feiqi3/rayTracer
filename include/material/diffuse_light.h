#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H

#include "material/material.h"
#include "math/vector.h"
#include "texture/constant_color.h"
#include "texture/texture.h"
#include <memory>
class diffuse_light :public material {
    public: 
    diffuse_light(std::shared_ptr<texture> _emit):emit(_emit){}
    diffuse_light(color c):emit(make_shared<constant_color>(c)){}
    bool scatter(const ray &_in, const record &hit_rec, color &out_attenuation, ray &out_scattered) const override{
        return false;
    }
    vec3 emitted(double u, double v, const vec3 &p) const override{
        return emit->value(u, v, p);
    }
    public: 
    shared_ptr<texture> emit;
};

#endif
