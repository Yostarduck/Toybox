#pragma once

#include "tb_vector3.h"
#include "tb_vector4.h"


namespace toyboxSDK {

Vector3::Vector3(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
    z = 1.0f;
  }
}

Vector3::Vector3(const Vector3& V) : x(V.x), y(V.y), z(V.z) {}

Vector3::Vector3(const Vector4& V) : x(V.x), y(V.y), z(V.z) {}

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3::~Vector3() {}

float
Vector3::dot(const Vector3& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z);
} 

Vector3
Vector3::cross(const Vector3& B) const {
  return Vector3(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x);
}

float
Vector3::length() const {
  return Math::sqrt(dot(*this));
}

float
Vector3::lengthSqr() const {
  return dot(*this);
}

Vector3&
Vector3::normalize() {
  TB_ASSERT(length() != 0.0f);
  *this /= length();
  return *this;
}

float
Vector3::distance(const Vector3& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector3::distanceSqr(const Vector3& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector3::equals(const Vector3& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange) &&
         (Math::abs(z - otherVector.z) < errorRange);
}

float
Vector3::sqrDistSegment(const Vector3& pointA, const Vector3& pointB) const {
  Vector3 AB = pointB - pointA;
  Vector3 AC = *this - pointA;
  Vector3 BC = *this - pointB;
  float e = AC.dot(AB);

  if (e <= 0.0f) {
   return AC.dot(AC);
  }

  float f = AB.dot(AB);
  
  if (e >= f) {
   return BC.dot(BC);
  }
  
  return AC.dot(AC) - (e * e / f);
}

float*
Vector3::ptr() {
  return &data[0];
}

const float*
Vector3::ptr() const {
  return &data[0];
}

float&
Vector3::operator[](const SizeT index){
  TB_ASSERT(index < 3);
  return data[index];
}

const float&
Vector3::operator[](const SizeT index) const {
  TB_ASSERT(index < 3);
  return data[index];
}

float
Vector3::operator|(const Vector3& B) const {
  return dot(B);
}

Vector3
Vector3::operator^(const Vector3& B) const {
  return cross(B);
}

Vector3&
Vector3::operator=(const Vector3& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  return *this;
}

Vector3&
Vector3::operator=(const Vector4& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  return *this;
}

Vector3
Vector3::operator+(const Vector3& A) const {
  return Vector3(x + A.x, y + A.y, z + A.z);
}

Vector3&
Vector3::operator+=(const Vector3& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  return *this;
}

Vector3
Vector3::operator-(const Vector3& A) const {
  return Vector3(x - A.x, y - A.y, z - A.z);
}

Vector3&
Vector3::operator-=(const Vector3& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  return *this;
}

Vector3
Vector3::operator*(const Vector3& A) const {
  return Vector3(x*A.x, y*A.y, z*A.z);
}

Vector3&
Vector3::operator*=(const Vector3& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  return *this;
}

Vector3
Vector3::operator*(const float scalar) const {
  return Vector3(x*scalar, y*scalar, z*scalar);
}

Vector3&
Vector3::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}

Vector3
Vector3::operator/(const float scalar) const {
  TB_ASSERT(scalar != 0.0f);
  return Vector3(x/ scalar, y/ scalar, z/ scalar);
}

Vector3&
Vector3::operator/=(const float scalar) {
  TB_ASSERT(scalar != 0.0f);
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return *this;
}

bool
Vector3::operator==(const Vector3& otherVector) const {
  return ((x == otherVector.x) && (y == otherVector.y) && (z == otherVector.z));
}

bool
Vector3::operator!=(const Vector3& otherVector) const {
  return !((*this) == otherVector);
}

Vector3
Vector3::operator-() const {
  return Vector3(-x, -y, -z);
}

}