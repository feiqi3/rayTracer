/*
 * @Author: feiqi3
 * @Date: 2022-01-24 15:27:35
 * @LastEditTime: 2022-03-29 14:37:53
 * @LastEditors: feiqi3
 * @Description: |Vector class|
 * @FilePath: \rayTracer\include\math\vector.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "tool/common.h"
#include <cmath>
#include <iostream>

class vec3 {
public:
  float vec[3];

public:
  vec3() : vec{0, 0, 0} {}
  vec3(float x, float y, float z) : vec{x, y, z} {};
  vec3(float x) : vec{x, x, x} {}
  float x() const { return vec[0]; }
  float y() const { return vec[1]; }
  float z() const { return vec[2]; }

  vec3 operator-() { return vec3(-vec[0], -vec[1], -vec[2]); }

  vec3 &operator+=(const vec3 &v) {
    vec[0] += v[0];
    vec[1] += v[1];
    vec[2] += v[2];
    return *this;
  }

  vec3 &operator*=(const double t) {
    vec[0] *= t;
    vec[1] *= t;
    vec[2] *= t;
    return *this;
  }

  vec3 &operator/=(const double t) { return *this *= 1.0 / t; }

  float length() const {
    return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
  }

  float operator[](int i) const { return vec[i]; }
  float &operator[](int i) { return vec[i]; }

  bool is_close_to_zero() {
    const auto bias = 1e-7;
    return (fabs(vec[0]) < bias) && (fabs(vec[1]) < bias) &&
           (fabs(vec[2]) < bias);
  }
};

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << "(" << v.vec[0] << "," << v.vec[1] << "," << v.vec[2] << ")";
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.vec[0] + v.vec[0], u.vec[1] + v.vec[1], u.vec[2] + v.vec[2]);
}

inline vec3 operator+(const vec3 &u, const double &v) {
  return u + vec3(v, v, v);
}

inline vec3 operator-(const vec3 &a, const vec3 &b) {
  return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

// return (u[0]*v[0]，u[1]*v[1],u[2]*v[2])
inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(const vec3 &u, double b) {
  return vec3(u[0] * b, u[1] * b, u[2] * b);
}

inline vec3 operator*(double a, const vec3 &u) { return u * a; }

inline vec3 operator/(const vec3 &u, const double &b) {
  double tmp = 1 / b;
  return u * tmp;
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
              u[0] * v[1] - u[1] * v[0]);
}

inline float dot(const vec3 &u, const vec3 &v) {
  return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 random_vec(double min, double max) {
  return vec3(rand_d(min, max), rand_d(min, max), rand_d(min, max));
}

// from(-1 to 1)
inline vec3 random_vec() { return random_vec(-1, 1); }

inline vec3 normalize(const vec3 &v) { return (v / v.length()); }

inline vec3 rand_in_unit_sphere() {
  while (true) {
    vec3 ans = random_vec();
    if (ans.length() < 1) {
      return normalize(ans);
    }
  }
}

inline vec3 get_rand_in_disk() {
  vec3 res;
  while (true) {
    res = vec3(rand_d(-1, 1), rand_d(-1, 1), 0);
    if (res.length() >= 1) {
      break;
    }
  }
  return res;
}

inline vec3 reflection(const vec3 &in, const vec3 &normal) {
  return in - 2 * dot(in, normal) * normal;
}

// to caculator refration ray
// eta of ray_in side divide eta of refractor side
inline vec3 refract(const vec3 &ray_in, const vec3 &ray_normal,
                    const double etai_divide_etat) {
  auto cos_theta = -dot(ray_normal, ray_in);
  return -(ray_normal * InvSqrt(1 - etai_divide_etat * etai_divide_etat *
                                        (1 - cos_theta * cos_theta))) +
         etai_divide_etat * (ray_in + ray_normal * cos_theta);
}

inline vec3 lerp(const vec3 &a, const vec3 &b, float i) {
  return vec3(std::lerp(a.x(), b.x(), i), std::lerp(a.y(), b.y(), i),
              std::lerp(a.z(), b.z(), i));
}

typedef vec3 color;

#endif