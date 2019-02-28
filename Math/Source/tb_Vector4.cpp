/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_Vector4.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/13 2019
 * @brief Vector implementation
 *
 */
 /*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#include "tb_vector4.h"

namespace toyboxSDK {

using std::isnan;
using std::isinf;
using std::pow;
using std::sqrt;

Vector4::Vector4(Vector3 og) {
  x = og.x;
  y = og.y;
  z = og.z;
  w = 1.0f;
}

float&
Vector4::operator[](UInt32 index) {
  return (&x)[index];
}

float
Vector4::operator[](UInt32 index) const {
  return (&x)[index];
}

Vector4
Vector4::operator+(const Vector4& v) const {
  return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4
Vector4::operator-(const Vector4& v) const {
  return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4
Vector4::operator*(const Vector4& v) const {
  return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4
Vector4::operator/(const Vector4& v) const {
  return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4
Vector4::operator+(float plus) const {
  return Vector4(x + plus, y + plus, z + plus, w + plus);
}

Vector4
Vector4::operator-(float minus) const {
  return Vector4(x - minus, y - minus, z - minus, w - minus);
}
Vector4
Vector4::operator*(float times) const {
  return Vector4(x * times, y * times, z * times, w * times);
}
Vector4
Vector4::operator/(float under) const {
  return Vector4(x / under, y / under, z / under, w / under);
}
float
Vector4::operator|(const Vector4 v) const {
  return x * v.x + y * v.y + z * v.z + w * v.w;
}

Vector4
Vector4::operator^(const Vector4 v) const {

  Vector4 tmp;

  tmp.x = (y*v.z) - (z*v.y);
  tmp.y = (z*v.x) - (x*v.z);
  tmp.z = (x*v.y) - (y*v.x);
  tmp.w = 1.0f;
  return tmp;
}

bool
Vector4::operator==(const Vector4& v) const {
  return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool
Vector4::operator!=(const Vector4& v) const {
  return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool
Vector4::operator<(const Vector4& v) const {
  return x < v.x && y < v.y && z < v.z && w < v.w;
}

bool
Vector4::operator>(const Vector4& v) const {
  return x > v.x && y > v.y && z > v.z && w > v.w;
}

bool
Vector4::operator<=(const Vector4& v) const {
  return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
}

bool
Vector4::operator>=(const Vector4& v) const {
  return x >= v.x && y >= v.y && z >= v.z && w >= v.w;
}

Vector4
Vector4::operator-() const {
  return Vector4(-x, -y, -z, -w);
}

Vector4&
Vector4::operator+=(const Vector4& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  w += v.w;
  return *this;
}

Vector4&
Vector4::operator-=(const Vector4& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  w -= v.w;
  return *this;
}

Vector4&
Vector4::operator/=(const Vector4& v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Vector4&
Vector4::operator*=(const Vector4& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  w *= v.w;
  return *this;
}

Vector4&
Vector4::operator*=(float scale) {
  x *= scale;
  y *= scale;
  z *= scale;
  w *= scale;
  return *this;
}

Vector4&
Vector4::operator/=(float scale) {
  x /= scale;
  y /= scale;
  z /= scale;
  w /= scale;
  return *this;
}

Vector4
Vector4::cross(const Vector4& a, const Vector4& b) {
  return a ^ b;
}

float
 Vector4::dot(const Vector4& a, const Vector4& b) {
  return a | b;
}

float
Vector4::dotScale(const Vector4& a, const Vector4& b) {
  return (a | b) / a.magnitude();
}

float
Vector4::sqrDistance(const Vector4& a, const Vector4& b) {
  return pow(a.x - b.x, 2.0f) + 
         pow(a.y - b.y, 2.0f) + 
         pow(a.z - b.z, 2.0f) +
         pow(a.z - b.z, 2.0f);
}

float
Vector4::distance(const Vector4& a, const Vector4& b) {
  return sqrt(pow(a.x - b.x, 2.0f) + 
              pow(a.y - b.y, 2.0f) +
              pow(a.z - b.z, 2.0f) +
              pow(a.w - b.w, 2.0f));

}

void
Vector4::set(float newX, float newY, float newZ, float newW = 1.0f) {
  x = newX;
  y = newY;
  z = newZ;
  w = newW;
}

void
Vector4::round() {
  x = std::round(x);
  y = std::round(y);
  z = std::round(z);
  w = std::round(w);
}

float
Vector4::magnitude() const {
  return sqrt(x * x + y * y + z * z + w * w);
}

float
Vector4::sqrMagnitude() const {
  return (x * x + y * y + z * z + w * w);
}

Vector4
  Vector4::normalized() const {

  TB_ASSERT(!isnan(x) &&
            !isnan(y) &&
            !isnan(z) &&
            !isnan(w) &&
            !isinf(x) &&
            !isinf(y) &&
            !isinf(z) &&
            !isinf(w) &&
            "Value X or Y or Z or W are either infinite or NAN");

  float sqr = pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f);

  TB_ASSERT(sqr <= std::numeric_limits<float>::epsilon() &&
            "Square is less than epsilon and that shit is wack");

  TB_DEBUG_ONLY(sqrMagnitude());

  float unit = 1.0f / sqrt(sqr);
  return Vector4((x * unit), (y * unit), (z * unit), (w * unit));
}

void
  Vector4::normalize() {
  TB_ASSERT(!isnan(x) &&
            !isnan(y) &&
            !isnan(z) &&
            !isnan(w) &&
            !isinf(x) &&
            !isinf(y) &&
            !isinf(z) &&
            !isinf(w) &&
            "Value X or Y or Z or W are either infinite or NAN");

  float sqr = pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f);

  TB_ASSERT(sqr <= std::numeric_limits<float>::epsilon() &&
            "Square is less than epsilon and that shit is wack");

  TB_DEBUG_ONLY(sqrMagnitude());

  float unit = 1.0f / sqrt(sqr);

  x *= unit;
  y *= unit;
  z *= unit;
  w *= unit;
}

const Vector4 Vector4::ONES   = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

const Vector4 Vector4::ZERO   = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

const Vector4 Vector4::RIGHT  = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

const Vector4 Vector4::UP     = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

const Vector4 Vector4::FRONT  = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
}
