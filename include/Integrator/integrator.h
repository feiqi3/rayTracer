#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "buffer/RGB12Buffer.h"
#include "camera.h"
#include "material/material.h"
#include "math/vector.h"
#include "object/bvh_node.h"
#include "object/hitable.h"
#include "ray.h"
#include "tool/common.h"
#include "tool/picTool.h"
#include <iostream>
#include <memory>
#include <stdlib.h>

const float TMIN = 0.001;
const float TMAX = 10000.;
const float max_dep = 15;

class integrator {
public:
  integrator(bvh_node *h, int img_height, int img_width, camera *_cam,
             int sample_nums)
      : world(h), height(img_height), width(img_width), samples(sample_nums),
        cam(_cam) {}
  virtual vec3 Li(ray r, record &rec, int depth) const = 0;
  virtual void Render() = 0;

protected:
  bvh_node *world;
  int height, width, samples;
  camera *cam;
};

class baseIntegrator : public integrator {
public:
  baseIntegrator(bvh_node *h, int img_height, int img_width, int sample_nums,
                 camera *_cam, const vec3 &_background)
      : integrator(h, img_height, img_width, _cam, sample_nums),
        backgroundColor(_background) {}
  vec3 backgroundColor;

public:
  void Render() override {
    srand(time(0));
    RGB12 mainBuffer(width, height);
    float inv_samples = 1. / samples;
    float division_x = 1. / width;
    float division_y = 1. / height;
    color pxl(0, 0, 0);
    float start_time = clock();
    int x = 0, y = 0;
    for (y = height - 1; y >= 0; --y) {
      for (x = 0; x < width; ++x) {
        for (std::size_t sample_times = 0; sample_times < samples;
             sample_times++) {
          // transform to NDC(?)
          auto u = (x + rand_d(-1, 1)) * division_x;
          auto v = (y + rand_d(-1, 1)) * division_y;
          ray tmp_ray = cam->get_ray(u, v);
          record rec;
          vec3 li = Li(tmp_ray, rec, max_dep);
          pxl +=
              (isnan(li.x()) || isnan(li.y()) || isnan(li.z())) ? vec3(0) : li;
        }
        pxl = pxl * inv_samples;
        mainBuffer.writeBuffer(x, y, pxl);
        pxl = vec3(0);
      }
    }
    float end_time = clock();
    std::cout << (end_time - start_time) / 60000 << "mins\n";
    fPic::jpgWriter(&mainBuffer);
    std::cout << "Done!";
  }
  vec3 Li(ray r, record &rec, int depth) const override {
    if (depth <= 0)
      return 0;
    if (!integrator::world->hit(r, TMIN, TMAX, rec)) {
      return backgroundColor;
    }
    if (depth == max_dep && rec.mat_ptr->getType() == material::Light) {
      ++Light_hit;
    }
    vec3 emission = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    vec3 wh;
    vec3 next_dir = rec.mat_ptr->sample_f(r.direction(), &wh, rec);

    vec3 f = rec.mat_ptr->f(r.dir, wh, next_dir, rec);
    if (f.x() == 0 && f.y() == 0 && f.z() == 0)
      return vec3(0);
    float cosTheta = absDot(rec.normal, next_dir);
    float pdf_ = rec.mat_ptr->pdf(next_dir, wh, rec);
    if (abs(pdf_ - 0) < 0.0001)
      return vec3(0);
    ray nx(rec.p, next_dir);

    vec3 res = emission + f * cosTheta * Li(nx, rec, depth - 1) / pdf_;
    return res;
  }

  // Debug
  void RenderPxl(int x, int y, int nsamples, int depth) {
    RGB12 mainBuffer(width, height);
    float inv_samples = 1. / nsamples;
    float division_x = 1. / width;
    float division_y = 1. / height;
    color pxl(0, 0, 0);
    for (int _y = height - 1; _y >= 0; --_y) {
      int _x = 0;
      for (_x = 0; _x < width; ++_x) {
        if (_x == x && _y == y)
          break;
        for (std::size_t sample_times = 0; sample_times < samples;
             sample_times++) {
          auto u = (x + rand_d(-1, 1)) * division_x;
          auto v = (y + rand_d(-1, 1)) * division_y;
          ray tmp_ray = cam->get_ray(u, v);
          record rec;
          vec3 li = Li(tmp_ray, rec, max_dep);
        }
      }
      if (_x == x && _y == y)
        break;
    }

    for (std::size_t sample_times = 0; sample_times < nsamples;
         sample_times++) {
      Light_hit = 0;
      // transform to NDC(?)
      auto u = (x + rand_d()) * division_x;
      auto v = (y + rand_d()) * division_y;
      ray tmp_ray = cam->get_ray(u, v);
      record rec;
      vec3 LI_ = Li(tmp_ray, rec, depth);
      std::cout << "sample:" << sample_times << "-->Target pixel color is "
                << LI_ << "\nLight Hit Time : " << Light_hit << "\n";
      pxl += LI_;
    }
    pxl = pxl * inv_samples;
    std::cout << "Target pixel color is " << pxl * 256 << "\n";
  }

private:
  mutable int Light_hit;
};

#endif