/*
 * @Author: feiqi3
 * @Date: 2022-05-15 10:43:14
 * @LastEditTime: 2022-05-15 10:47:47
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\HeaderFiles.h
 * ->blog: feiqi3.cn <-
 */
#pragma once
#include "tool/common.h"
#include "math/vector.h"
#include "ray.h"
#include "tool/ppmUtil.h"
#include "tool/picTools.h"
#include "buffer/RGB12Buffer.h"
#include "renderPass/camera.h"
#include "renderPass/subRender.h"
#include <memory>
#include "hitableList.h"
#include "object/triangle.h"
#include "object/sphere.h"
#include "object/hitable.h"
#include "material/normal_shader.h"
#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"