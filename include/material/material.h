/*
 * @Author: feiqi3
 * @Date: 2022-03-02 13:44:22
 * @LastEditTime: 2022-05-12 12:32:01
 * @LastEditors: feiqi3
 * @Description: |material class|
 * @FilePath: \rayTracer\include\material\material.h
 * ->blog: feiqi3.cn <-
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../math/vector.h"
#include "../ray.h"
struct record;

class material {
public:
virtual bool scatter(const ray& _in,const record& hit_rec,color& out_attenuation,ray& out_scattered) const = 0;
};

#endif