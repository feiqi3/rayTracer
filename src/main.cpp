/*
 * @Author: feiqi3
 * @Date: 2022-01-24 20:06:53
 * @LastEditTime: 2022-05-24 22:38:34
 * @LastEditors: feiqi3
 * @Description: |main application|
 * @FilePath: \rayTracer\src\main.cpp
 * ->blog: feiqi3.cn <-
 */
#include "../include/HeaderFiles.h"
#include "Macro.h"
#include "RenderQueue.h"
#include "buffer/RGB12Buffer.h"
#include "material/texture.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "object/model.h"
#include "object/scene.h"
#include "object/sphere.h"
#include "object/texture_rectangle.h"
#include "object/texture_triangle.h"
#include "presets.h"
#include "renderPass/renderPass.h"
#include "tool/fLoader.h"
#include <iostream>
#include <memory>


constexpr int IMG_WIDTH = 1920;
constexpr double RATIO = 16.0 / 9.0;
constexpr int SAMPLES = 300;

int main() {
  Flog logger();
  Flog::set_glob_log_level(INFO);

  int img_width = IMG_WIDTH;
  int img_height = static_cast<int>(img_width / RATIO);
  vec3 camPos(0, 0, 10);
  vec3 lookAt(0, 0, -1);
  auto rq = renderQueue::getInstance();
  // auto sc =
  //    std::make_shared<scene>(vec3(0, 0, 0), 300, "./skyBox/Pandora [From
  //    tutorial]/");

  rq->set(nullptr, IMG_WIDTH, 16. / 9, true);
  rq->sample_times = SAMPLES;
  PreSets::Three_Little_Ballgys();
  floader f;
  rq->setThreadNum(16);
  rq->MultiThreadRender();
  rq->SaveToFile();
}
