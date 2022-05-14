/*
 * @Author: feiqi3
 * @Date: 2022-01-26 14:08:25
 * @LastEditTime: 2022-05-10 15:05:33
 * @LastEditors: feiqi3
 * @Description: |ray class|
 * @FilePath: \raytracer_post\rayTracer\include\ray.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _RAY_H_
#define _RAY_H_
#include "math/vector.h"
class ray
{
public:
    vec3 orig;
    vec3 dir;

public:
    ray() {}
    ray(const vec3 &_orig, const vec3 &_dir)
        : orig(_orig), dir(normalize(_dir))
    {
    }

    vec3 origin()const { return orig; }
    vec3 direction()const { return dir; }

    vec3 at(double t)const
    {
        return t*dir + orig;
    }
};
#endif