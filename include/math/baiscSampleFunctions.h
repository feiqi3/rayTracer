#ifndef BASIC_SAMPLE_FUN_
#define BASIC_SAMPLE_FUN_
#include "vector.h"
#include "tool/common.h"
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
        t = b / a * inv_pi4;
    }else {
        r = b;
        t = inv_pi2 -a / b *  inv_pi4;
    }
    *x = r * cos(t);
    *y = r * sin(t);
}


inline vec3 sampleCosWeighted(){
    float x,y;
    ConcentricSampleDisk(&x, &y);;
    float z = sqrt(fmax(0.,1 - x * x - y * y));
    return vec3(x,z,y);
}

#endif