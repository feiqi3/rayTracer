/*
 * @Author: feiqi3
 * @Date: 2022-05-18 12:23:28
 * @LastEditTime: 2022-05-18 14:39:18
 * @LastEditors: feiqi3
 * @Description: |---description here---|
 * @FilePath: \rayTracer\include\RenderQueue.h
 * ->blog: feiqi3.cn <-
 */
#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include "buffer/Buffer.h"
#include "buffer/RGB12Buffer.h"
#include "hitableList.h"
#include "math/vector.h"
#include "renderPass/camera.h"
#include "renderPass/renderPass.h"
#include "tool/picTools.h"
#include <cstddef>
#include <memory>
#include <vector>

struct renderPair {
  std::shared_ptr<renderPass> renderPass;
  std::shared_ptr<buffer> renderBuffer;
};

// Singleton
class renderQueue {
  // Implement of Singleton
private:
  static std::shared_ptr<renderQueue> _INSTANCE;

public:
  static std::shared_ptr<renderQueue> getInstance();

public:
  renderQueue();
  renderQueue(const std::shared_ptr<renderPass> & main_cam, int imgWidth, float _ratio, bool _open_ssao,
              int sample_nums, const char *savepath, const char *savename);
  void setMainRender(const std::shared_ptr<renderPass> &);
  void addObj(const std::shared_ptr<object> &obj);
  void setOutput(const char *);
  int addSubRenderPass(const std::shared_ptr<renderPass> &, int x, int y,
                       BUFFER_TPYE _buffer_type);
  void Render() const;
  void SaveToFile() const;

protected:
  hitable_list world;
  renderPair main_cam;
  std::vector<renderPair> subRenderList;
  const char *filePath;
  const char *fileName;
  const bool SSAA_OPEN;
  float ratio;
  int img_width;
  int img_height;
  int sampleNums;
  int maxRayDepth;
};

inline renderQueue::renderQueue()
    : ratio(16. / 9), img_width(1000), img_height(img_width / ratio),
      SSAA_OPEN(true), fileName(nullptr), filePath(nullptr) {}

inline renderQueue::renderQueue(const std::shared_ptr<renderPass> & main_cam, int imgWidth, float _ratio,
                                bool _open_ssao = true, int sample_nums = 64,
                                const char *savepath = nullptr, const char *savename = nullptr)
    : SSAA_OPEN(_open_ssao), ratio(_ratio), img_width(imgWidth),
      img_height(img_width / ratio), fileName(savename), filePath(savepath),maxRayDepth(100),sampleNums(sample_nums) 
      {
          setMainRender(main_cam);
      }

std::shared_ptr<renderQueue> renderQueue::_INSTANCE = nullptr;
inline std::shared_ptr<renderQueue> renderQueue::getInstance() {
  if (_INSTANCE == nullptr) {
    return std::make_shared<renderQueue>();
  } else {
    return _INSTANCE;
  }
}

inline void renderQueue::addObj(const std::shared_ptr<object> &obj) {
  world.add(obj);
}

inline int
renderQueue::addSubRenderPass(const std::shared_ptr<renderPass> &pass, int x,
                              int y, BUFFER_TPYE _buffer_type) {
  renderPair __passNbuffer;
  __passNbuffer.renderPass = pass;
  if (_buffer_type == FBUFFER_RGB12F)
    __passNbuffer.renderBuffer = std::make_shared<RGB12F>(x, y);
  if (_buffer_type == FBUFFER_RGB12)
    __passNbuffer.renderBuffer = std::make_shared<RGB12>(x, y);
  __passNbuffer.renderBuffer->make_buffer();
  subRenderList.push_back(__passNbuffer);
  return subRenderList.size() - 1;
}

inline void
renderQueue::setMainRender(const std::shared_ptr<renderPass> &main_render) {
  main_cam.renderPass = main_render;
  main_cam.renderBuffer = std::make_shared<RGB12>(img_width, img_height);
  main_cam.renderBuffer->make_buffer();
}

inline void renderQueue::Render() const {
  double division_x = 1.0 / (img_width - 1.0);
  double division_y = 1.0 / (img_height - 1.0);
  for (int y = img_height - 1; y >= 0; --y) {
    for (int x = 0; x < img_width; ++x) {
      color pxl(0, 0, 0);
      double tmp_x = x * division_x;
      double tmp_y = y * division_y;
      for (auto i = subRenderList.begin(); i != subRenderList.end(); i++) {
        ray tmp = i->renderPass->get_ray(tmp_x, tmp_y);
        color clr = i->renderPass->cast_ray(tmp, world, maxRayDepth);
        i->renderBuffer->writeBuffer(x, y, clr);
      }
       if (SSAA_OPEN) {
        for (std::size_t sample_times = 0; sample_times < 64; sample_times++) {
          // transform to NDC(?)

          auto u = (x + rand_d()) * division_x;
          auto v = (y + rand_d()) * division_y;
          ray tmp_ray = main_cam.renderPass->get_ray(u, v);
          pxl += main_cam.renderPass->cast_ray(tmp_ray, world, maxRayDepth);
          main_cam.renderBuffer->writeBuffer(x, y, (pxl * 1. / sampleNums));
        }
      } else {
        ray tmp_ray = main_cam.renderPass->get_ray(tmp_x, tmp_y);
        pxl = main_cam.renderPass->cast_ray(tmp_ray, world, maxRayDepth);
        main_cam.renderBuffer->writeBuffer(x, y, (pxl));
      }
      // pm.colorWrite(main_fram->sampler(x * division_x, y * division_y));
      //      pm.m_s_colorWirte(pxl,divided_samples);
    }
    if (y % ((int)(img_height / 50)) == 0) {
      Flog::flog(TRACE, "render :"
                + STR((int)((1.0 - (double)y / (img_height - 1.0)) * 100)));
    }
  }
}

void inline renderQueue::SaveToFile() const {
  if (filePath != nullptr && fileName != nullptr)
    fPic::jpgWriter(main_cam.renderBuffer.get(), filePath, fileName, 1);
  else
    fPic::jpgWriter(main_cam.renderBuffer.get());
}

#endif