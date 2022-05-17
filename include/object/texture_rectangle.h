/*
 * @Author: feiqi3
 * @Date: 2022-05-17 09:56:29
 * @LastEditTime: 2022-05-17 10:03:40
 * @LastEditors: feiqi3
 * @Description: |---rectangle with texture!---|
 * @FilePath: \rayTracer\include\object\texture_rectangle.h
 * ->blog: feiqi3.cn <-
 */
#ifndef TEXTURE_rectangle_H
#define TEXTURE_rectangle_H

#include "../material/texture.h"
#include "../buffer/RGB12Buffer.h"
#include "hitable.h"
#include "object/triangle.h"
class texture_rectangle:public hitable{
    texture_rectangle(const RGB12& _buf);
    virtual bool hit(const ray &r, double t_min, double t_max,
                   record &rec) const override;
    private:triangle tri1,tri2;
    RGB12 tex_buf;
};

#endif