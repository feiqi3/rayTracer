/*
 * @Author: feiqi3
 * @Date: 2022-03-02 13:44:22
 * @LastEditTime: 2022-03-03 20:32:52
 * @LastEditors: feiqi3
 * @Description: |material class|
 * @FilePath: \rayTracer\include\material\material.h
 * ->blog: feiqi3.cn <-
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "math/vector.h"
#include "ray.h"
struct record;

class material {
public:
virtual bool scatter(const ray& _in,const record& hit_rec,color& out_attenuation,ray& out_scattered) const = 0;

virtual vec3 emitted(double u,double v,const vec3& p)const{
    return vec3(0);
    }
};

#endif