#pragma once

#include "tb_vector2.h"

namespace toyboxSDK {

Vector2::Vector2() {}

Vector2::Vector2(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    x = 0.0f;
    y = 0.0f;
  }
  else {
    x = 1.0f;
    y = 1.0f;
  }
}

Vector2::Vector2(const Vector2& V) : x(V.x), y(V.y) {}

Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

Vector2::~Vector2() {}

float
Vector2::dot(const Vector2& B) const {
  return (x*B.x) + (y*B.y);
}

Vector2
Vector2::cross() const {
  return Vector2(x, -y);
}

float
Vector2::length() const {
  return Math::sqrt(dot(*this));
}

float
Vector2::lengthSqr() const {
  return dot(*this);
}

Vector2&
Vector2::normalize() {
  TB_ASSERT(length() != 0.0f);
  *this /= length();
  return *this;
}

float
Vector2::distance(const Vector2& otherVector) const {
  return (otherVector - *this).length();
}

float
Vector2::distanceSqr(const Vector2& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector2::equals(const Vector2& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange);
}

Int32
Vector2::compare(const Vector2& p1, const Vector2& p2) {
  Int32 compOrientation = orientation(p1, p2);

  if (compOrientation == 0) {
    if (distanceSqr(p1) < distanceSqr(p2)) {
      return 1;
    }
    else {
      return -1;
    }
  }

  if (compOrientation == 2) {
    return -1;
  }
  else {
    return 1;
  }
}

Int32
Vector2::orientation(const Vector2& q, const Vector2& r) const {
  float val = (q.y - y) * (r.x - q.x) - (q.x - x) * (r.y - q.y);

  if (val == 0.0f) {
    return 0;  //Colinear
  }
  else if (val > 0.0f) {
    return 1;
  }
  else {
    return 2; //Clock or counterclock wise
  }
}

float*
Vector2::ptr() {
  return &data[0];
}

const float*
Vector2::ptr() const {
  return &data[0];
}

float&
Vector2::operator[](const SizeT index){
  TB_ASSERT(index < 2);
  return data[index];
}

const float&
Vector2::operator[](const SizeT index) const {
  TB_ASSERT(index < 2);
  return data[index];
}

float
Vector2::operator|(const Vector2& B) const {
  return dot(B);
}


Vector2&
Vector2::operator=(const Vector2& A) {
  x = A.x;
  y = A.y;
  return *this;
}

Vector2
Vector2::operator+(const Vector2& A) const {
  return Vector2(x + A.x, y + A.y);
}

Vector2&
Vector2::operator+=(const Vector2& A) {
  x += A.x;
  y += A.y;
  return *this;
}

Vector2
Vector2::operator-(const Vector2& A) const {
  return Vector2(x - A.x, y - A.y);
}

Vector2&
Vector2::operator-=(const Vector2& A) {
  x -= A.x;
  y -= A.y;
  return *this;
}

Vector2
Vector2::operator*(const Vector2& A) const {
  return Vector2(x*A.x, y*A.y);
}

Vector2&
Vector2::operator*=(const Vector2& A) {
  x *= A.x;
  y *= A.y;
  return *this;
}

Vector2
Vector2::operator*(const float scalar) const {
  return Vector2(x*scalar, y*scalar);
}

Vector2&
Vector2::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

Vector2
Vector2::operator/(const float scalar) const {
  TB_ASSERT(scalar != 0.0f);
  return Vector2(x / scalar, y / scalar);
}

Vector2&
Vector2::operator/=(const float scalar) {
  TB_ASSERT(scalar != 0.0f);
  x /= scalar;
  y /= scalar;
  return *this;
}

bool
Vector2::operator==(const Vector2& otherVector) {
  return (x == otherVector.x) && (y == otherVector.y);
}

bool
Vector2::operator!=(const Vector2& otherVector) {
  return !((*this) == otherVector);
}

Vector2
Vector2::operator-() const {
  return Vector2(-x, -y);
}

}