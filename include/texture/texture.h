#ifndef TEXTURE_H
#define TEXTURE_H

#include "math/vector.h"
class texture{
    public:
    virtual vec3 value(double u,double v,const vec3& point) const = 0;
};

#endif
