#ifndef BASIC_SAMPLE_FUN_
#define BASIC_SAMPLE_FUN_
#include "vector.h"
#include "tool/common.h"
#include <algorithm>
#include <cmath>
#include <stdlib.h>

inline void uniformSampleDisk(float* x,float* y){
    float r = sqrt(rand_d());
    float theta = pi * 2 *rand_d();
    *x = r * cos(theta);
    *y = r * sin(theta);
}
inline void ConcentricSampleDisk(float* x,float* y){
    float a = rand_d(-1,1);
    float b = rand_d(-1,1);
    float r,t;
    if(abs(a) > abs(b)){
        r = a;
        t = b / a * inv_4pi;
    }else {
        r = b;
        t = inv_2pi -a / b *  inv_4pi;
    }
    *x = r * cos(t);
    *y = r * sin(t);
}


inline vec3 sampleCosWeighted(){
    float x,y;
    uniformSampleDisk(&x, &y);;
    float z = sqrt(std::max(0.0001f,1 - x * x - y * y));
    return vec3(x,z,y);
}

#endif