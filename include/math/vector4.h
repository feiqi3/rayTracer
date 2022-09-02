/*
 * @Author: feiqi3
 * @Date: 2022-01-24 15:27:35
 * @LastEditTime: 2022-05-21 11:32:34
 * @LastEditors: feiqi3
 * @Description: |Vector class|
 * @FilePath: \rayTracer\include\math\vector4.h
 * ->blog: feiqi3.cn <-
 */
#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include "../tool/common.h"
#include "vector.h"
#include <cmath>
#include <iostream>
#include <sstream>

class vec4 {
public:
  float vec[4];

public:
  vec4() : vec{0, 0, 0} {}
  vec4(float x):vec{x,x,x,x}{}
  vec4(float x, float y, float z, float w) : vec{x, y, z, w} {};
  vec4(vec3 v, float w) : vec{v.x(), v.y(), v.z(), w} {};
  vec4(float x, vec3 v) : vec{x, v.x(), v.y(), v.z()} {};

  float x() const { return vec[0]; }
  float y() const { return vec[1]; }
  float z() const { return vec[2]; }
  float w() const { return vec[3]; }
  vec3 xyz() const{ return vec3(x(), y(), z()); }
  std::string const toString() const {
    std::ostringstream oos;
    oos << x() << "," << y() << "," << z() << "," << w();
    return "(" + oos.str() + ")";
  }

  vec4 operator-() const{

    return vec4(-vec[0], -vec[1], -vec[2], -vec[3]);
  }

  vec4 &operator+=(const vec4 &v) {
    vec[0] += v[0];
    vec[1] += v[1];
    vec[2] += v[2];
    vec[3] += v[3];

    return *this;
  }

  vec4 &operator*=(const double t) {
    vec[0] *= t;
    vec[1] *= t;
    vec[2] *= t;
    vec[3] *= t;
    return *this;
  }

  vec4 &operator/=(const double t) { return *this *= 1.0 / t; }

  double length() const {
    return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] +
                vec[3] * vec[3]);
  }

  float operator[](int i) const { return vec[i]; }
  float &operator[](int i) { return vec[i]; }
};

inline std::ostream &operator<<(std::ostream &out, const vec4 &v) {
  return out << "(" << v.vec[0] << "," << v.vec[1] << "," << v.vec[2] << ")";
}

inline vec4 operator+(const vec4 &u, const vec4 &v) {
  return vec4(u.vec[0] + v.vec[0], u.vec[1] + v.vec[1], u.vec[2] + v.vec[2],
              u.vec[3] + v.vec[3]);
}

inline vec4 operator+(const vec4 &u, const double &v) {
  return u + vec4(v, v, v, v);
}

inline vec4 operator-(const vec4 &a, const vec4 &b) {
  return vec4(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}

// return component-wise multi
inline vec4 operator*(const vec4 &u, const vec4 &v) {
  return vec4(u[0] * v[0], u[1] * v[1], u[2] * v[2], u[3] * v[3]);
}

inline vec4 operator*(const vec4 &u, double b) {
  return vec4(u[0] * b, u[1] * b, u[2] * b, u[3] * b);
}

inline vec4 operator*(double a, const vec4 &u) { return u * a; }

inline vec4 operator/(const vec4 &u, const double &b) {
  double tmp = 1 / b;
  return u * tmp;
}

inline vec4 normalize(const vec4 &v) { return (v / v.length()); }

#endif