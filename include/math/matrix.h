#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include "vector4.h"
#include <string>

class mat4 {
public:
  vec4 cmp[4];
  mat4(vec4 a, vec4 b, vec4 c, vec4 d) {
    cmp[0] = a;
    cmp[1] = b;
    cmp[2] = c;
    cmp[3] = d;
  }

  vec4 &operator[](int index) { return cmp[index]; }

  static mat4 Identity();

  std::string toString() {
    return "Mat4:\n" + cmp[0].toString() + "\n" + cmp[1].toString() + "\n" +
           cmp[2].toString() + "\n" + cmp[3].toString();
  }
};

inline mat4 IdentityMat() {
  return mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0),
              vec4(0, 0, 0, 1));
}

inline mat4 mat4::Identity() { return IdentityMat(); }

inline vec4 __getCol(mat4 a, int i) {
  return vec4(a.cmp[0].vec[i], a.cmp[1].vec[i], a.cmp[2].vec[i],
              a.cmp[3].vec[i]);
}

inline float __cmp_wise_mul_then_add(const vec4 a, const vec4 &b) {
  vec4 tmp = a * b;
  return tmp.x() + tmp.y() + tmp.z() + tmp.w();
}

// Matrix4 dot
inline mat4 operator*(const mat4 &a, const mat4 &b) {
  double m00 = __cmp_wise_mul_then_add(a.cmp[0], __getCol(b, 0));
  double m01 = __cmp_wise_mul_then_add(a.cmp[0], __getCol(b, 1));
  double m02 = __cmp_wise_mul_then_add(a.cmp[0], __getCol(b, 2));
  double m03 = __cmp_wise_mul_then_add(a.cmp[0], __getCol(b, 3));
  double m10 = __cmp_wise_mul_then_add(a.cmp[1], __getCol(b, 0));
  double m11 = __cmp_wise_mul_then_add(a.cmp[1], __getCol(b, 1));
  double m12 = __cmp_wise_mul_then_add(a.cmp[1], __getCol(b, 2));
  double m13 = __cmp_wise_mul_then_add(a.cmp[1], __getCol(b, 3));
  double m20 = __cmp_wise_mul_then_add(a.cmp[2], __getCol(b, 0));
  double m21 = __cmp_wise_mul_then_add(a.cmp[2], __getCol(b, 1));
  double m22 = __cmp_wise_mul_then_add(a.cmp[2], __getCol(b, 2));
  double m23 = __cmp_wise_mul_then_add(a.cmp[2], __getCol(b, 3));
  double m30 = __cmp_wise_mul_then_add(a.cmp[3], __getCol(b, 0));
  double m31 = __cmp_wise_mul_then_add(a.cmp[3], __getCol(b, 1));
  double m32 = __cmp_wise_mul_then_add(a.cmp[3], __getCol(b, 2));
  double m33 = __cmp_wise_mul_then_add(a.cmp[3], __getCol(b, 3));
  // Fuck me
  return mat4(vec4(m00, m01, m02, m03), vec4(m10, m11, m12, m13),
              vec4(m20, m21, m22, m23), vec4(m30, m31, m32, m33));
}

inline vec4 operator*(const mat4 &mat, const vec4 &vec) {
  double a = __cmp_wise_mul_then_add(mat.cmp[0], vec);
  double b = __cmp_wise_mul_then_add(mat.cmp[1], vec);
  double c = __cmp_wise_mul_then_add(mat.cmp[2], vec);
  double d = __cmp_wise_mul_then_add(mat.cmp[3], vec);
  return vec4(a, b, c, d);
}

namespace mat {

inline mat4 getTranslate(const vec3 &vec) {
  mat4 tmp = IdentityMat();
  tmp.cmp[0].vec[3] = vec[0];
  tmp.cmp[1].vec[3] = vec[1];
  tmp.cmp[2].vec[3] = vec[2];
  return tmp;
}

inline mat4 getScale(const vec3 &vec) {
  mat4 tmp = IdentityMat();
  tmp.cmp[0].vec[0] = vec[0];
  tmp.cmp[1].vec[1] = vec[1];
  tmp.cmp[2].vec[2] = vec[2];
  return tmp;
}

// Rodrigues's rotation formula
inline mat4 getRotate(double theta, const vec3 &axis) {
  double a = 1 - cos(theta);
  double s = sin(theta);
  double c = cos(theta);
  double m00 = a * axis.x() * axis.x() + c;
  double m01 = a * axis.x() * axis.y() - s * axis.z();
  double m02 = a * axis.x() * axis.z() + s * axis.y();
  double m03 = 0;
  double m10 = a * axis.y() * axis.x() + s * axis.z();
  double m11 = a * axis.y() * axis.y() + c;
  double m12 = a * axis.y() * axis.z() - s * axis.x();
  double m13 = 0;
  double m20 = a * axis.z() * axis.x() - s * axis.y();
  double m21 = a * axis.z() * axis.y() + s * axis.x();
  double m22 = a * axis.z() * axis.z() + c;
  double m23 = 0;

  // Kill me
  return mat4(vec4(m00, m01, m02, m03), vec4(m10, m11, m12, m13),
              vec4(m20, m21, m22, m23), vec4(0, 0, 0, 1));
}
} // namespace mat
#endif