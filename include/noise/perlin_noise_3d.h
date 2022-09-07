#ifndef PERLIN_NOISE_3D_H
#define PERLIN_NOISE_3D_H

#include "math/vector.h"
#include "tool/common.h"
#include <cmath>
#include <corecrt_math.h>

class perlin3D {
public:
  perlin3D();
  ~perlin3D();
  //[-1,1]
  double noise(const vec3 &p)const;
  double noise01(const vec3&p)const;
  double turb_noise(vec3 p,int layer)const;
private:
  vec3 *ranVec;
  // Hash seed in x,y,z
  int *perm_x;
  int *perm_y;
  int *perm_z;
  static constexpr int point_count = 256;
  static constexpr int mask = point_count - 1;
  // ease curve function
  static double easeCurve(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
  }
  static vec3 easeCurve(const vec3 &p) {
    return vec3(easeCurve(p.x()), easeCurve(p.y()), easeCurve(p.z()));
  }

  static double trilinear_interp(double c[2][2][2], const vec3 &);

  // Generate hash seed array
  static int *perlin_generate_perm();

  // Shuffle the array
  static void permute(int *p, int n);
};

inline double perlin3D::trilinear_interp(double c[2][2][2], const vec3 &weight) {
  auto accum = 0.0;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      for (int k = 0; k < 2; k++)
        accum += (i * weight.x() + (1 - i) * (1 - weight.x())) *
                 (j * weight.y() + (1 - j) * (1 - weight.y())) *
                 (k * weight.z() + (1 - k) * (1 - weight.z())) * c[i][j][k];

  return accum;
}

inline perlin3D::perlin3D() {
  perm_x = perlin_generate_perm();
  perm_y = perlin_generate_perm();
  perm_z = perlin_generate_perm();
  ranVec = new vec3[point_count];
  for (int i = 0; i < point_count; ++i) {
    ranVec[i] = normalize(random_vec());
  }
}

inline perlin3D::~perlin3D() {
  delete[] ranVec;
  delete[] perm_x;
  delete[] perm_y;
  delete[] perm_z;
}

inline int *perlin3D::perlin_generate_perm() {
  auto p = new int[point_count];
  for (int i = 0; i < perlin3D::point_count; i++)
    p[i] = i;
  permute(p, point_count);
  return p;
}

inline void perlin3D::permute(int *p, int n) {
  for (int i = n - 1; i > 0; i--) {
    int target = rand_i(0, i);
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}

inline double perlin3D::noise(const vec3 &p)const {
  vec3 pi = vec3(floorf(p.x()), floorf(p.y()), floorf(p.z()));
  vec3 pf = p - pi;
  vec3 weight = easeCurve(pf);
  double c[2][2][2];
  for (int di = 0; di < 2; di++)
    for (int dj = 0; dj < 2; dj++)
      for (int dk = 0; dk < 2; dk++) {
        vec3 corner(di, dj, dk);
        vec3 c2p = pf - corner;
        int hash_ = perm_x[((int)pi.x()+di) & mask] ^ perm_y[((int)pi.y()+dj) & mask] ^
                    perm_z[((int)pi.z()+dk) & mask];
        auto &grad = ranVec[hash_];
        c[di][dj][dk] = dot(c2p, grad);
      }
  return  trilinear_interp(c, weight);
}

inline double perlin3D::noise01(const vec3 &p)const{
  return noise(p) * 0.5 + 0.5;
}

inline double perlin3D::turb_noise(vec3 p,int layer = 8)const{
  float amp = 1;
  double res = 0;
  for (int i = 0; i < layer; i++) {
    res += noise(p)*amp;
    amp *=0.5;
    p = p*2;//double frequency
  }
  return fabs(res);
}
#endif // PERLIN_NOISE_2D_H