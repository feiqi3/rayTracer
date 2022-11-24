#include "material.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "object/hitable.h"
#include "texture/texture.h"
#include "tool/common.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstdlib>

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
  float CosThetah, Cos2Thetah;
  float SinThetah, Sin2Thetah;
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
  float G1(const vec3 &w) const { return 1. / (1 + GGX_G_lambda(w)); }

private:
  vec3 sampleVNDF(vec3 *wh, record &hit_rec) const;
  vec3 sampleP11(const vec3 &wo, record &hit_rec) const;
  vec3 sampleP22(const vec3 &wo, record &hit_rec) const;

  vec3 shlick(const vec3 &F0) {
    return F0 + (vec3(1) - F0) * pow(1 - CosThetah, 5);
  }
  float GGX(const vec3 &wh, const vec3 &wi, const record &rec) const {
    float cos2 = Cos2Thetah;
    auto kk = cosTheta2PHIh / (r.x() * r.x()) + sinTheta2PHIh / (r.y() * r.y());
    kk = 1 + Sin2Thetah / cos2 * kk;
    kk = kk * kk;
    float res = inv_pi / (r.x() * r.y() * cos2 * cos2 * kk);
    assert(!isinf(res));
    assert(!isnan(res));
    return res;
  }

  // In normal space
  float GGX_G_lambda(const vec3 &w) const {
    vec3 wo_nor = w;
    float tanThetaO = absDot(wo_nor, tbn[1].xyz());
    float phi = 0;
    if (dot(tbn[1].xyz(), wo_nor) > 0.9999) {
      phi = acos(clamp(
          dot(normalize(-tanThetaO * tbn[1].xyz() + wo_nor), tbn[0].xyz()), -1.,
          1.));
    }
    tanThetaO = tanThetaO * tanThetaO / (1 - tanThetaO * tanThetaO);
    float absTanTheta2 = tanThetaO;
    float Cos2PhiW = cos(phi);
    Cos2PhiW *= Cos2PhiW;
    float Sin2PhiW = 1 - Cos2PhiW;
    float alpha =
        std::sqrt(Cos2PhiW * r.x() * r.x() + Sin2PhiW * r.y() * r.y());
    float alpha2Tan2Theta = alpha * alpha * absTanTheta2;
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
  }

  vec3 sample_GGX_NDF(vec3 *wh, record &hit_rec) const {
    float rd1 = rand_d();
    float rd2 = rand_d();
    float phi, theta;
    vec3 res;
    if (r.x() == r.y()) {
      float phi = 2 * pi * rd1;
      float t2t = rd2 * r.x() * r.x() / (1 - rd2);
      float cost = 1 / sqrt(1 + t2t);
      float sint = sqrt(std::max(0.f, 1 - cost));
      res = SphericalToXYZ(sint, cost, phi);
    } else {
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
  CosThetah = absDot(w_h, hit_rec.normal);
  Cos2Thetah = CosThetah * CosThetah;
  SinThetah = cross(w_h, hit_rec.normal).length();
  Sin2Thetah = SinThetah * SinThetah;
  cosThetaPHIh = SinThetah == 0.
                     ? 1.
                     : clamp(dot(w_h, tbn[0].xyz()) / SinThetah, -1.,
                             1.); // w.x / sinTheta
  cosTheta2PHIh = cosThetaPHIh * cosThetaPHIh;
  sinThetaPHIh = SinThetah == 0.
                     ? 0.
                     : clamp(dot(w_h, tbn[2].xyz()) / SinThetah, -1.,
                             1.); // w.z / sinTheta
  sinTheta2PHIh = sinThetaPHIh * sinThetaPHIh;
  float dotA = std::max(absDot(hit_rec.normal, w_o), 0.001);
  float dotB = std::max(absDot(hit_rec.normal, w_i), 0.001);
  vec3 fel = Fresnel(F0);
  float g2 = G2(-1 * w_o, w_i);
  float ndf = NDF(w_h, w_i, hit_rec);
  vec3 res = _mat->value(hit_rec.u, hit_rec.v, hit_rec.p) * inv_pi +
             ndf * fel * g2 * 0.25f / (dotA * dotB);
  assert(!hasnanV(res));
  return res;
}

inline float microfacet::pdf(const vec3 &wi, const vec3 &wh,
                             const record &rec) const {
  // Transfrorm pdf from wh to wi
  return NDF(wh, wi, rec) * CosThetah * 0.25f / absDot(wi, wh);
}

// https://agraphicsguynotes.com/posts/sample_anisotropic_microfacet_brdf/
// Sample_f must run before f to initialize tbn.
inline vec3 microfacet::sample_f(const vec3 &wo, vec3 *wh,
                                 record &hit_rec) const {
  tbn = mat::getTBNFromXY(hit_rec.normal);
  auto tmp = sampleP22(wo, hit_rec);
  if (tmp.y() < 0)
    tmp = -tmp;
  *wh = normalize(tmp * tbn);
  return normalize(reflection(wo, *wh));
}

inline vec3 microfacet::sampleP11(const vec3 &wo_nor, record &hit_rec) const {
  float rd1 = rand_d(0.00002, 0.99998);
  float rd2 = rand_d(0.00002, 0.99998);
  float slope_x, slope_y;

  if (absDot(wo_nor, tbn[1].xyz()) > 0.9999) {
    float r = sqrt(rd1 / (1 - rd1));
    float phi = 6.28318530718 * rd2;
    slope_x = r * cos(phi);
    slope_y = r * sin(phi);
    return vec3(slope_x, 0, slope_y);
  }
  float tanThetaO = clamp(absDot(tbn[1].xyz(), wo_nor), -1., 1.);
  tanThetaO = sqrt(1 - pow2(tanThetaO)) / tanThetaO;
  float g1 = 2. / (1. + sqrt(1 + pow2(tanThetaO)));
  assert(!isnan(tanThetaO));

  assert(!isnan(g1));
  float A = 2 * rd1 / (g1)-1;
  assert(!isnan(A) );
  float B = tanThetaO;
  // sample slope_x
  float tmp = 1. / (A * A - 1.);
  assert(!isnan(tmp));
  float D = sqrt(B * B * tmp * tmp - (A * A - B * B) * tmp);
  assert(!isnan(D));
  const float slope_x_1 = B * tmp - D;
  const float slope_x_2 = B * tmp + D;
  slope_x = (A < 0 || slope_x_2 > 1.0 / tanThetaO) ? slope_x_1 : slope_x_2;
  if(isnan(slope_x)){
    int x =0;
  }
  assert(!isnan(slope_x));
  // sample slope_y
  float S;
  if (rd2 > .5) {
    S = 1.0;
    rd2 = 2 * (rd2 - .5);
  } else {
    S = -1.;
    rd2 = 2 * (.5 - rd2);
  }
  float Z = (rd2 * (rd2 * (rd2 * 0.27385 - 0.73369) + 0.46341)) /
            (rd2 * (rd2 * (rd2 * 0.093073 + 0.309420) - 1.000000) + 0.5);
  slope_y = S * Z * sqrt(1 + slope_x * slope_x);
  assert(!isnan(slope_y));
  return vec3(slope_x, 0, slope_y);
}

inline vec3 microfacet::sampleP22(const vec3 &wo, record &hit_rec) const {
  vec3 wo_nor = wo;
  float cosThetaO;
  if ((cosThetaO = dot(wo_nor, tbn[1].xyz())) < 0) {
    wo_nor = -wo_nor;
    cosThetaO = -cosThetaO;
  }
  // 1.stretch the light
  vec3 alpha = _rough->value(hit_rec.u, hit_rec.v, hit_rec.p);
  wo_nor[0] *= alpha.x();
  wo_nor[2] *= alpha.y();
  wo_nor = wo_nor / wo_nor.length();
  // 2.sample p22
  vec3 sample = sampleP11(wo_nor, hit_rec);
  // 3.rotate
  float phi = 0;
  if (dot(tbn[1].xyz(), wo_nor) > 0.9999) {
    float tmp_dot =
        dot(normalize(-cosThetaO * tbn[1].xyz() + wo_nor), tbn[0].xyz());
    phi = acos(clamp(tmp_dot, -1.f, 1.f));
  }
  sample[0] = cos(phi) * sample.x() - sin(phi) * sample.z();
  sample[2] = sin(phi) * sample.x() + cos(phi) * sample.z();
  // 4.unstretch the sample point
  sample[0] *= alpha.x();
  sample[2] *= alpha.y();
  // 5.compute normal
  float inv_m = sqrt(sample.x() * sample.x() + sample.z() * sample.z() + 1.0);
  assert(!isnan(!inv_m));
  sample[0] = -sample[0] / inv_m;
  sample[2] = -sample[2] / inv_m;
  sample[1] = 1. / inv_m;
  assert(!hasnanV(sample));
  return sample;
}

#endif