#include "material.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "object/hitable.h"
#include "texture/texture.h"
#include "tool/common.h"
#include <algorithm>
#include <cmath>
#include <assert.h>
#ifndef MICROFACET_H_
#define MICROFACET_H_

class microfacet : public material {
public:
  microfacet(const std::shared_ptr<texture> &mat,
             const std::shared_ptr<texture> &roughness, const vec3 &_F0)
      : _rough(roughness), _mat(mat), F0(_F0) {}
  vec3 f(const vec3 &w_o, const vec3 &w_h, const vec3 &w_i,
         record &hit_rec) override;
  float pdf(const vec3 &wi, const vec3 &wh, const record &rec) const override;
  vec3 sample_f(const vec3 &wo, vec3 *wh, record &hit_rec) const override;
  bool scatter(const ray &_in, const record &hit_rec, color &out_attenuation,
               ray &out_scattered) const override {
    return false;
  }
  MType getType() const override { return Microfacet; }

protected:
  std::shared_ptr<texture> _rough;
  std::shared_ptr<texture> _mat;
  float CosTheta, Cos2Theta;
  float SinTheta, Sin2Theta;
  float cosThetaPHIh, cosTheta2PHIh;
  float sinThetaPHIh, sinTheta2PHIh;
  mutable vec3 r;
  vec3 F0;
  mutable mat4 tbn;

  float NDF(const vec3 &wh, const vec3 &wi, const record &rec) const {
    return GGX(wh, wi, rec);
  }
  vec3 Fresnel(const vec3 &F0) { return shlick(F0); }
  float G2(const vec3 &wo, const vec3 &wi) { return G1(wo) * G1(wi); }
  float G1(const vec3 &w) { return 1. / (1 + GGX_G_lambda(w)); }

private:
  vec3 shlick(const vec3 &F0) { return F0 + (vec3(1) - F0) * pow(1 - CosTheta, 5); }
  float GGX(const vec3 &wh, const vec3 &wi, const record &rec) const {
    float cos2 = Cos2Theta;
    auto kk = cosTheta2PHIh / (r.x() * r.x()) + sinTheta2PHIh / (r.y() * r.y());
    kk = 1 + Sin2Theta / cos2 * kk;
    kk = kk * kk;
    return inv_pi / (r.x() * r.y() * cos2 * cos2 * kk);
  }

  float GGX_G_lambda(const vec3 &w) {
    float absTanTheta2 = std::abs(Sin2Theta) / Cos2Theta;
    float Cos2PhiW = dot(tbn[0].xyz(), w);
    Cos2PhiW *= Cos2PhiW;
    float Sin2PhiW = cross(tbn[0].xyz(), w).length();
    Sin2PhiW *= Sin2PhiW;
    float alpha =
        std::sqrt(Cos2PhiW * r.x() * r.y() + Sin2PhiW * r.x() * r.y());
    float alpha2Tan2Theta = alpha * alpha * abs(Sin2Theta / Cos2Theta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
  }

  vec3 sample_GGX_NDF(vec3 *wh, record &hit_rec) const {
    float rd1 = rand_d();
    float rd2 = rand_d();
    float phi, theta;
    vec3 res;
    if (r.x() == r.y()) {
      float phi = 2 *pi * rd1;
      float t2t = rd2 * r.x()*r.x() / (1 - rd2);
      float cost = 1 / sqrt(1+t2t);
      float sint = sqrt(std::max(0.f,1-cost));
      res = SphericalToXYZ(sint, cost, phi);
    }
    else{
      r = _rough->value(hit_rec.u, hit_rec.v, hit_rec.p);
      // sample phi
      // phi is in [-1/2 pi,3/2 pi] , but we will "cos" it then
      float alpha = r.y() / r.x();
      if (rd1 > 0.5)
        phi = atan(alpha * tan(doublePi * rd1 + 0.5f * pi)) + pi;
      else
        phi = atan(alpha * tan(doublePi * rd1 + 0.5f * pi));
      float t =
          pow2(cos(phi)) / (r.x() * r.x()) + pow2(sin(phi)) / (r.y() * r.y());
      float t2t = rd2 / (t * (1 - rd2)); // tan2theta
      float cost = 1 / std::sqrt(1 + t2t);
      float sint = sqrt(std::max(0.f, 1.f - cost * cost));
      res = SphericalToXYZ(sint, cost, phi);
    }
    return res;
  }
};
// from PBRT

inline vec3 microfacet::f(const vec3 &w_o, const vec3 &w_h, const vec3 &w_i,
                          record &hit_rec) {
  if (dot(hit_rec.normal, w_i) < 0)
    return vec3(0);
  r = _rough->value(hit_rec.u, hit_rec.v, hit_rec.p);
  CosTheta = absDot(w_h, hit_rec.normal);
  Cos2Theta = CosTheta * CosTheta;
  SinTheta = cross(w_h, hit_rec.normal).length();
  Sin2Theta = SinTheta * SinTheta;
  cosThetaPHIh = SinTheta == 0. ? 1.
                                : clamp(dot(w_h, tbn[0].xyz()) / SinTheta, -1.,
                                        1.); // w.x / sinTheta
  cosTheta2PHIh = cosThetaPHIh * cosThetaPHIh;
  sinThetaPHIh = SinTheta == 0. ? 0.
                                : clamp(dot(w_h, tbn[2].xyz()) / SinTheta, -1.,
                                        1.); // w.z / sinTheta
  sinTheta2PHIh = sinThetaPHIh * sinThetaPHIh;
  float dotA = std::max(absDot(hit_rec.normal, w_o),0.001);
  float dotB =std::max(absDot(hit_rec.normal, w_i),0.001);
  vec3 fel = Fresnel(F0);
  float g2 = G2(-1 * w_o, w_i);
  float ndf = NDF(w_h, w_i, hit_rec);
  vec3 res = _mat->value(hit_rec.u, hit_rec.v, hit_rec.p) * inv_pi +
             ndf * fel * g2 * 0.25f /
                 (dotA * dotB);
  return res;
}

inline float microfacet::pdf(const vec3 &wi, const vec3 &wh,
                             const record &rec) const {
  // Transfrorm pdf from wh to wi
  return NDF(wh, wi, rec) * CosTheta * 0.25f / absDot(wi, wh);
}

// https://agraphicsguynotes.com/posts/sample_anisotropic_microfacet_brdf/
//Sample_f must run before f to initialize tbn.
inline vec3 microfacet::sample_f(const vec3 &wo, vec3 *wh,
                                 record &hit_rec) const {
  tbn = mat::getTBNFromXY(hit_rec.normal);
  auto tmp = sample_GGX_NDF(wh, hit_rec);
  if (tmp.y() < 0)
    tmp = -tmp;
  *wh = normalize(tmp * tbn);
  return normalize(reflection(wo, *wh));
}

#endif