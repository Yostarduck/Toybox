#pragma once

#include "tb_matrix3x3.h"
#include "tb_matrix4x4.h"

namespace toyboxSDK {

Matrix3x3::Matrix3x3() {}

Matrix3x3::Matrix3x3(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    *this = zerosMat3x3;
  }
  //kIdentity
  else {
    *this = identityMat3x3;
  }
}

Matrix3x3::Matrix3x3(const Matrix4x4& M)
  : vector0(M.vector0),
    vector1(M.vector1),
    vector2(M.vector2) {}
/*
Matrix3x3::Matrix3x3(const Quaternion& Q) {
  float q1pow2 = Q.x * Q.x;
  float q2pow2 = Q.y * Q.y;
  float q3pow2 = Q.z * Q.z;
  float q0q1 = Q.w * Q.x;
  float q0q2 = Q.w * Q.y;
  float q0q3 = Q.w * Q.z;
  float q1q2 = Q.x * Q.y;
  float q1q3 = Q.x * Q.z;
  float q2q3 = Q.y * Q.z;

  data[0][0] = 1.f - 2.f * (q2pow2 + q3pow2);
  data[0][1] = 2.f * (q1q2 - q0q3);
  data[0][2] = 2.f * (q0q2 + q1q3);

  data[1][0] = 2.f * (q1q2 + q0q3);
  data[1][1] = 1.f - 2.f * (q1pow2 + q3pow2);
  data[1][2] = 2.f * (q2q3 - q0q1);

  data[2][0] = 2.f * (q1q3 - q0q2);
  data[2][1] = 2.f * (q0q1 + q2q3);
  data[2][2] = 1.f - 2.f * (q1pow2 + q2pow2);
}
*/

Matrix3x3::Matrix3x3(const Matrix3x3& M)
  : vector0(M.vector0),
    vector1(M.vector1),
    vector2(M.vector2) {}

Matrix3x3::Matrix3x3(float v0x, float v0y, float v0z,
                     float v1x, float v1y, float v1z,
                     float v2x, float v2y, float v2z)
  : vector0(v0x, v0y, v0z),
    vector1(v1x, v1y, v1z),
    vector2(v2x, v2y, v2z) {}

Matrix3x3::Matrix3x3(Vector3 vector0, Vector3 vector1, Vector3 vector2)
  : vector0(vector0),
    vector1(vector1),
    vector2(vector2) {}

Matrix3x3::~Matrix3x3() {}

float 
Matrix3x3::determinant() const {
  return (vector0.x * vector1.y * vector2.z + 
          vector0.y * vector1.z * vector2.x +
          vector0.z * vector1.x * vector2.y) -
         (vector2.x * vector1.y * vector0.z +
          vector2.y * vector1.z * vector0.x +
          vector2.z * vector1.x * vector0.y);
}

Matrix3x3&
Matrix3x3::cofactor() {
  Matrix3x3 temp;
  
  temp.vector0.x = vector1.y * vector2.z - vector2.y * vector1.z;
  temp.vector0.y = vector2.x * vector1.z - vector1.x * vector2.z;
  temp.vector0.z = vector1.x * vector2.y - vector2.x * vector1.y;

  temp.vector1.x = vector2.y * vector0.z - vector0.y * vector2.z;
  temp.vector1.y = vector0.x * vector2.z - vector2.x * vector0.z;
  temp.vector1.z = vector2.x * vector0.y - vector0.x * vector2.y;

  temp.vector2.x = vector0.y * vector1.z - vector1.y * vector0.z;
  temp.vector2.y = vector1.x * vector0.z - vector0.x * vector1.z;
  temp.vector2.z = vector0.x * vector1.y - vector1.x * vector0.y;

  *this = temp;
  return *this;
}

Matrix3x3&
Matrix3x3::adjugate() {
  cofactor();
  transpose();
  return *this;
}

Matrix3x3&
Matrix3x3::inverse() {
  float deter = determinant();
  TB_ASSERT(deter != 0);
  adjugate();
  *this *= (1.f / deter);
  return *this;
}

Matrix3x3&
Matrix3x3::transpose() {
  Matrix3x3 temp;
  temp.vector0.x = vector0.x;
  temp.vector1.x = vector0.y;
  temp.vector2.x = vector0.z;

  temp.vector0.y = vector1.x;
  temp.vector1.y = vector1.y;
  temp.vector2.y = vector1.z;

  temp.vector0.z = vector2.x;
  temp.vector1.z = vector2.y;
  temp.vector2.z = vector2.z;

  *this = temp;
  return *this;
}

Matrix3x3&
Matrix3x3::identity() {
  *this = identityMat3x3;
  return *this;
}

bool
Matrix3x3::equals(const Matrix3x3& otherMatrix, float errorRange) const
{
  return data[0].equals(otherMatrix.data[0], errorRange) &&
         data[1].equals(otherMatrix.data[1], errorRange) &&
         data[2].equals(otherMatrix.data[2], errorRange);
}

Vector3
Matrix3x3::eulerAngles()
{
  Vector3 temp;
  temp.x = Math::aTan2(data[2][1], data[2][2]);
  float c = Math::sqrt(data[2][1] * data[2][1] + data[2][2] * data[2][2]);
  temp.y = Math::aTan2(-data[2][0], c);
  temp.z = Math::aTan2(data[1][0], data[0][0]);
  return temp;
}

float*
Matrix3x3::ptr() {
  return data[0].ptr();
}

const float*
Matrix3x3::ptr() const {
  return data[0].ptr();
}

Vector3&
Matrix3x3::operator[](SizeT index){
  TB_ASSERT(index < 3);
  return data[index];
}

Matrix3x3&
Matrix3x3::operator=(const Matrix3x3& A) {
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  return *this;
}

Matrix3x3&
Matrix3x3::operator=(const Matrix4x4& A)
{
  vector0 = A.vector0;
  vector1 = A.vector1;
  vector2 = A.vector2;
  return *this;
}

Matrix3x3
Matrix3x3::operator+(const Matrix3x3& A) const {
  return Matrix3x3(vector0 + A.vector0,
                   vector1 + A.vector1,
                   vector2 + A.vector2);
}

Matrix3x3&
Matrix3x3::operator+=(const Matrix3x3& A) {
  vector0 += A.vector0;
  vector1 += A.vector1;
  vector2 += A.vector2;
  return *this;
}

Matrix3x3
Matrix3x3::operator-(const Matrix3x3& A) const {
  return Matrix3x3(vector0 - A.vector0,
                   vector1 - A.vector1,
                   vector2 - A.vector2);
}

Matrix3x3&
Matrix3x3::operator-=(const Matrix3x3& A) {
  vector0 -= A.vector0;
  vector1 -= A.vector1;
  vector2 -= A.vector2;
  return *this;
}

Matrix3x3
Matrix3x3::operator*(const Matrix3x3& A) const {
  Matrix3x3 temp;

  Vector3 c0(A.vector0.x, A.vector1.x, A.vector2.x);
  Vector3 c1(A.vector0.y, A.vector1.y, A.vector2.y);
  Vector3 c2(A.vector0.z, A.vector1.z, A.vector2.z);
  
  temp.vector0.x = vector0.dot(c0);
  temp.vector0.y = vector0.dot(c1);
  temp.vector0.z = vector0.dot(c2);

  temp.vector1.x = vector1.dot(c0);
  temp.vector1.y = vector1.dot(c1);
  temp.vector1.z = vector1.dot(c2);

  temp.vector2.x = vector2.dot(c0);
  temp.vector2.y = vector2.dot(c1);
  temp.vector2.z = vector2.dot(c2);

  return temp;
}

Matrix3x3&
Matrix3x3::operator*=(const Matrix3x3& A) {
  Matrix3x3 temp;

  Vector3 c0(A.vector0.x, A.vector1.x, A.vector2.x);
  Vector3 c1(A.vector0.y, A.vector1.y, A.vector2.y);
  Vector3 c2(A.vector0.z, A.vector1.z, A.vector2.z);

  temp.vector0.x = vector0.dot(c0);
  temp.vector0.y = vector0.dot(c1);
  temp.vector0.z = vector0.dot(c2);

  temp.vector1.x = vector1.dot(c0);
  temp.vector1.y = vector1.dot(c1);
  temp.vector1.z = vector1.dot(c2);

  temp.vector2.x = vector2.dot(c0);
  temp.vector2.y = vector2.dot(c1);
  temp.vector2.z = vector2.dot(c2);
  
  *this = temp;
  return *this;
}

Vector3
Matrix3x3::operator*(const Vector3& S) const {
  Vector3 temp;

  temp.x = vector0.dot(S);
  temp.y = vector1.dot(S);
  temp.z = vector2.dot(S);

  return temp;
}

Matrix3x3
Matrix3x3::operator*(const float S) const {
  Matrix3x3 temp;

  temp.vector0 = vector0 * S;
  temp.vector1 = vector1 * S;
  temp.vector2 = vector2 * S;

  return temp;
}

Matrix3x3&
Matrix3x3::operator*=(const float S) {
  vector0 *= S;
  vector1 *= S;
  vector2 *= S;

  return *this;
}

Matrix3x3
Matrix3x3::operator/(const Matrix3x3& M) const
{
  Matrix3x3 temp(M);
  return *this * temp.inverse();
}

Matrix3x3&
Matrix3x3::operator/=(const Matrix3x3& M)
{
  Matrix3x3 temp(M);
  *this *= temp.inverse();
  return *this;
}

bool
Matrix3x3::operator==(const Matrix3x3& M) {
  return vector0 == M.vector0 && vector1 == M.vector1 && vector2 == M.vector2;
}

bool
Matrix3x3::operator!=(const Matrix3x3& M) {
  return !(*this == M);
}

Matrix3x3
Matrix3x3::identityMat3x3(1.f, 0.f, 0.f,
                          0.f, 1.f, 0.f,
                          0.f, 0.f, 1.f);
Matrix3x3
Matrix3x3::zerosMat3x3(0.f, 0.f, 0.f,
                       0.f, 0.f, 0.f,
                       0.f, 0.f, 0.f);

}