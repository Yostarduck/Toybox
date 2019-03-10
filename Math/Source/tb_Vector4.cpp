#include "tb_vector4.h"
#include "tb_vector3.h"
#include "tb_vector2.h"
#include "tb_matrix4x4.h"

namespace toyboxSDK {

Vector4::Vector4() { }

Vector4::Vector4(Math::FORCE_INIT k) {
  if (Math::FORCE_INIT::kZero == k) {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }
  else {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
  }
}

Vector4::Vector4(const Vector4& V) : x(V.x), y(V.y), z(V.z), w(V.w) {}

Vector4::Vector4(const Vector3& V) : x(V.x), y(V.y), z(V.z) {
}

Vector4::Vector4(const Vector3& V, float _w) : x(V.x), y(V.y), z(V.z), w(_w) {
}

Vector4::Vector4(const Vector2& V1, const Vector2& V2)
  : x(V1.x),
    y(V1.y),
    z(V2.x),
    w(V2.y) {
}

Vector4::Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {
}

Vector4::~Vector4() {
}

float
Vector4::dot3(const Vector4& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z);
}

float
Vector4::dot(const Vector4& B) const {
  return (x*B.x) + (y*B.y) + (z*B.z) + (w*B.w);
}

Vector4
Vector4::cross(const Vector4& B) const {
  return Vector4(y*B.z - z*B.y, z*B.x - x*B.z, x*B.y - y*B.x, 0.0f);
}

float Vector4::length3() const {
  return Math::sqrt(dot3(*this));
}

float
Vector4::length() const {
  return Math::sqrt(dot(*this));
}

float Vector4::lengthSqr3() const {
  return dot3(*this);
}

float
Vector4::lengthSqr() const {
  return dot(*this);
}

void Vector4::normalize3() {
  TB_ASSERT(length() != 0);
  float Length3 = length3();
  x /= Length3;
  y /= Length3;
  z /= Length3;
}

void
Vector4::normalize() {
  TB_ASSERT(length() != 0);
  *this /= length();
}

float Vector4::distance3(const Vector4& otherVector) const {
  return (otherVector - *this).length3();
}

float
Vector4::distance(const Vector4& otherVector) const {
  return (otherVector - *this).length();
}

float Vector4::distanceSqr3(const Vector4& otherVector) const {
  return (otherVector - *this).lengthSqr3();
}

float
Vector4::distanceSqr(const Vector4& otherVector) const {
  return (otherVector - *this).lengthSqr();
}

bool
Vector4::equals(const Vector4& otherVector, float errorRange) const {
  return (Math::abs(x - otherVector.x) < errorRange) &&
         (Math::abs(y - otherVector.y) < errorRange) &&
         (Math::abs(z - otherVector.z) < errorRange) &&
         (Math::abs(w - otherVector.w) < errorRange);
}

float*
Vector4::ptr() {
  return &data[0];
}

const float*
Vector4::ptr() const {
  return &data[0];
}

float&
Vector4::operator[](const SizeT index){
  TB_ASSERT(index < 4);
  return data[index];
}

const float&
Vector4::operator[](const SizeT index) const {
  TB_ASSERT(index < 4);
  return data[index];
}

float
Vector4::operator|(const Vector4& B) const {
  return dot(B);
}

Vector4
Vector4::operator^(const Vector4& B) const {
  return cross(B);
}

Vector4&
Vector4::operator=(const Vector4& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  w = A.w;
  return *this;
}

Vector4&
Vector4::operator=(const Vector3& A) {
  x = A.x;
  y = A.y;
  z = A.z;
  return *this;
}

Vector4
Vector4::operator+(const Vector4& A) const {
  return Vector4(x + A.x, y + A.y, z + A.z, w + A.w);
}

Vector4&
Vector4::operator+=(const Vector4& A) {
  x += A.x;
  y += A.y;
  z += A.z;
  w += A.w;
  return *this;
}

Vector4
Vector4::operator-(const Vector4& A) const {
  return Vector4(x - A.x, y - A.y, z - A.z, w - A.w);
}

Vector4&
Vector4::operator-=(const Vector4& A) {
  x -= A.x;
  y -= A.y;
  z -= A.z;
  w -= A.w;
  return *this;
}

Vector4
Vector4::operator*(const Vector4& A) const {
  return Vector4(x*A.x, y*A.y, z*A.z, w * A.w);
}

Vector4 
Vector4::operator*(Matrix4x4 matrix) const {
  return matrix.transpose() * *this;
}

Vector4&
Vector4::operator*=(const Vector4& A) {
  x *= A.x;
  y *= A.y;
  z *= A.z;
  w *= A.w;
  return *this;
}

Vector4
Vector4::operator*(const float scalar) const {
  return Vector4(x*scalar, y*scalar, z*scalar, w*scalar);
}

Vector4&
Vector4::operator*=(const float scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  w *= scalar;
  return *this;
}

Vector4
Vector4::operator/(const float scalar) const {
  TB_ASSERT(scalar != 0.0f);
  return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vector4&
Vector4::operator/=(const float scalar) {
  TB_ASSERT(scalar != 0.0f);
  x /= scalar;
  y /= scalar;
  z /= scalar;
  w /= scalar;
  return *this;
}

bool
Vector4::operator==(const Vector4& otherVector) {
  return ((x == otherVector.x) &&
          (y == otherVector.y) &&
          (z == otherVector.z) &&
          (w == otherVector.w));
}

bool
Vector4::operator!=(const Vector4& otherVector) {
  return !((*this) == otherVector);
}

Vector4
Vector4::operator-() const {
  return Vector4(-x, -y, -z, -w);
}
}