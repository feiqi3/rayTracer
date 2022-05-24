/*
 * @Author: feiqi3
 * @Date: 2022-05-21 15:04:18
 * @LastEditTime: 2022-05-24 13:40:21
 * @LastEditors: feiqi3
 * @Description: |---Load Model!---|
 * @FilePath: \rayTracer\include\tool\fLoader.h
 * ->blog: feiqi3.cn <-
 */
#ifndef FLOADER_H_
#define FLOADER_H_
#include "../Vendor/OBJ_Loader.h"
#include "../math/vector.h"
#include "../object/model.h"
#include "cstring"
#include "flog.h"
#include <memory>

class floader {
public:
  floader() {}
  bool fload(const char *);
  std::shared_ptr<model> getModel(const char *);

protected:
  objl::Loader loader;
  vec3 V32vec3(const objl::Vector3 &V) { return vec3(V.X, V.Y, V.Z); }
  vec3 V22vec3(const objl::Vector2 &V) { return vec3(V.X, V.Y, 0); }
};

inline bool floader::fload(const char *_name) {
  Flog::flog(INFO, "Loading obj.\n");
  if (!loader.LoadFile(_name)) {
    Flog::flog(ERRO, "Load fail.");
    return false;
  }
  return true;
}

inline std::shared_ptr<model>
floader::getModel(const char *picPath = "__NULL") {

  std::shared_ptr<model> res = make_shared<model>();
  std::shared_ptr<RGB12> pic;
  // If texture map doesn't exist
  // then use material's albedo instead
  if (strcmp(picPath, "__NULL") == 0) {
    pic = nullptr;
  } else {
    pic = std::make_shared<RGB12>(picPath);
  }
  for (auto mesh : loader.LoadedMeshes) {
//clamp to 0,1


    // use color as albedo
    std::shared_ptr<lambertian> mat = std::make_shared<lambertian>(V32vec3(mesh.MeshMaterial.Kd));
    mat->ka =V32vec3( mesh.MeshMaterial.Ka);
    mat->ks =V32vec3(mesh.MeshMaterial.Ks);

    for (int i = 0; i < mesh.Indices.size() / 3; i++) {
      auto i1 = mesh.Indices[i * 3];
      auto i2 = mesh.Indices[i * 3 + 1];
      auto i3 = mesh.Indices[i * 3 + 2];
      vec3 v1 = V32vec3(mesh.Vertices[i1].Position);
      vec3 v2 = V32vec3(mesh.Vertices[i2].Position);
      vec3 v3 = V32vec3(mesh.Vertices[i3].Position);
      vec3 t1 = V22vec3(mesh.Vertices[i1].TextureCoordinate);
      vec3 t2 = V22vec3(mesh.Vertices[i2].TextureCoordinate);
      vec3 t3 = V22vec3(mesh.Vertices[i3].TextureCoordinate);
      Flog::flog(TRACE,"V1 "+v1.toString()+" ,V2 "+v2.toString() + " ,V3 "+v3.toString());
      std::shared_ptr<texture_triangle> tri = make_shared<texture_triangle>(v1, v2, v3, t1, t2, t3, pic, mat);
      tri->init();
      res->add(tri);
    }
  }
  return res;
}
#endif