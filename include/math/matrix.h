#ifndef MATRIX_H
#define MATRIX_H

#include "tool/common.h"
#include "math/vector.h"
#include "vector.h"
#include "vector4.h"
#include <cmath>
#include <iostream>
#include <string>
class mat4 {
public:
  vec4 cmp[4];
  mat4() {}
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

inline vec3 operator*(const mat4& mat,const vec3& vec){
  return (mat * vec4(vec,1)).xyz();
}

inline vec4 operator*(const vec4 &vec,const mat4 &mat) {
  double a = __cmp_wise_mul_then_add(__getCol(mat, 0), vec);
  double b = __cmp_wise_mul_then_add(__getCol(mat, 1), vec);
  double c = __cmp_wise_mul_then_add(__getCol(mat, 2), vec);
  double d = __cmp_wise_mul_then_add(__getCol(mat, 3), vec);
  return vec4(a, b, c, d);
}

inline vec3 operator*(const vec3& vec,const mat4& mat){
  return (vec4(vec,1) * mat).xyz();
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

inline mat4 getTBN(const vec3 &t, const vec3 &n) {
  vec3 tmp_t = normalize(t);
  vec3 bi =normalize(cross(n, t));
  tmp_t = normalize(cross(bi,n));
  return mat4(vec4(tmp_t, 0), vec4(n, 0), vec4(bi, 0), vec4(0));
}

/*
 * Implementation of Schmidt orthogonalization
 * @nor should be normalized
 * To fit the function sample_on_hemi,
 * I swaped the position of bitangent and normal . 
 * So actually this is a TNB matrix :-) .
 * To transform a point to tangent space, you should mult the mat with dir. 
 * Note: order does matter! 
 * You can prove it by caculating dot(tans_dir,n)>0 .
 */
inline mat4 getTBN(const vec3 &nor) {
  vec3 tmp = random_vec();
  auto d = (dot(tmp, nor) * nor);
  vec3 tangent = normalize(tmp - d);
  vec3 biTangent = cross(nor, tangent);
  return mat4(vec4(tangent, 0), vec4(nor, 0), vec4(biTangent, 0), vec4(0));
}

inline mat4 getTBNFromXY(const vec3 &nor){
  vec3 tan = vec3(1,0,0);
  if (absDot(tan,nor) >0.9f ) {
    tan = vec3(0,0,1);
  }
  return getTBN(tan,nor);
}

} // namespace mat

//y - [0,1] , xz - [-1,1]
inline vec3 sample_on_hemi() {
  float y = rand_d();
  float d = rand_d() * pi * 2;
  float r = sqrt(1 - y * y);
  return vec3(cos(d) * r, y, sin(d) * r);
}

inline vec3 sample_on_hemi(const vec3 &nor) {
  return normalize(sample_on_hemi() * mat::getTBN(nor) );
}

#endif