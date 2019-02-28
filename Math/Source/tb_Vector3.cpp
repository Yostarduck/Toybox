/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_Vector3.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/13 2019
 * @brief Vector implementation
 *
 */
 /*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#include "tb_vector3.h"

namespace toyboxSDK {

using std::isnan;
using std::isinf;
using std::pow;
using std::sqrt;

float&
Vector3::operator[](UInt32 index) {
  return (&x)[index];
}

float
Vector3::operator[](UInt32 index) const {
  return (&x)[index];
}

Vector3
Vector3::operator+(const Vector3& v) const {
  return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3
Vector3::operator-(const Vector3& v) const {
  return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3
Vector3::operator*(const Vector3& v) const {
  return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3
Vector3::operator/(const Vector3& v) const {
  return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3
Vector3::operator+(float plus) const {
  return Vector3(x + plus, y + plus, z +plus);
}

Vector3
Vector3::operator-(float minus) const {
  return Vector3(x - minus, y - minus, z - minus);
}
Vector3
Vector3::operator*(float times) const {
  return Vector3(x * times, y * times, z * times);
}
Vector3
Vector3::operator/(float under) const {
  return Vector3(x / under, y / under, z / under);
}
float
Vector3::operator|(const Vector3 v) const {
  return x * v.x + y * v.y + z * v.z;
}

Vector3
Vector3::operator^(const Vector3 v) const {

  Vector3 tmp;

  tmp.x = (y*v.z) - (z*v.y);
  tmp.y = (z*v.x) - (x*v.z);
  tmp.z = (x*v.y) - (y*v.x);

  return tmp;
}

bool
Vector3::operator==(const Vector3& v) const {
  return x == v.x && y == v.y && z == v.z;
}

bool
Vector3::operator!=(const Vector3& v) const {
  return x != v.x || y != v.y || z != v.z;
}

bool
Vector3::operator<(const Vector3& v) const {
  return x < v.x && y < v.y && z < v.z;
}

bool
Vector3::operator>(const Vector3& v) const {
  return x > v.x && y > v.y && z > v.z;
}

bool
Vector3::operator<=(const Vector3& v) const {
  return x <= v.x && y <= v.y && z <= v.z;
}

bool
Vector3::operator>=(const Vector3& v) const {
  return x >= v.x && y >= v.y && z >= v.z;
}

Vector3
Vector3::operator-() const {
  return Vector3(-x, -y, -z);
}

Vector3&
Vector3::operator+=(const Vector3& v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3&
Vector3::operator-=(const Vector3& v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3&
Vector3::operator/=(const Vector3& v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Vector3&
Vector3::operator*=(const Vector3& v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

Vector3&
Vector3::operator*=(float scale) {
  x *= scale;
  y *= scale;
  z *= scale;
  return *this;
}

Vector3&
Vector3::operator/=(float scale) {
  x /= scale;
  y /= scale;
  z /= scale;
  return *this;
}

Vector3
Vector3::cross(const Vector3& a, const Vector3& b) {
  return a ^ b;
}

float
Vector3::dot(const Vector3& a, const Vector3& b) {
  return a | b;
}

float
Vector3::dotScale(const Vector3& a, const Vector3& b) {
  return (a | b) / a.magnitude();
}

float
Vector3::sqrDistance(const Vector3& a, const Vector3& b) {
  return pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f) + pow(a.z - b.z, 2.0f);
}

float
Vector3::distance(const Vector3& a, const Vector3& b) {
  return sqrt(pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f) + pow(a.z - b.z, 2.0f));

}

void
Vector3::set(float newX, float newY, float newZ) {
  x = newX;
  y = newY;
  z = newZ;
}

void
Vector3::round() {
  x = std::round(x);
  y = std::round(y);
  z = std::round(z);
}

float
Vector3::magnitude() const {
  return sqrt(x * x + y * y + z * z);
}

float
Vector3::sqrMagnitude() const {
  return (x * x + y * y + z * z);
}

Vector3
Vector3::normalized() const {

  TB_ASSERT(!isnan(x) &&
            !isnan(y) &&
            !isnan(z) &&
            !isinf(x) &&
            !isinf(y) &&
            !isinf(z) &&
            "Value X or Y or Z are either infinite or NAN");

  float sqr = pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f);

  TB_ASSERT(sqr <= std::numeric_limits<float>::epsilon() &&
    "Square is less than epsilon and that shit is wack");

  TB_DEBUG_ONLY(sqrMagnitude());

  float unit = 1.0f / sqrt(sqr);
  return Vector3((x * unit), (y * unit), (z * unit));
}

void
Vector3::normalize() {
  TB_ASSERT(!isnan(x) &&
            !isnan(y) &&
            !isnan(z) &&
            !isinf(x) &&
            !isinf(y) &&
            !isinf(z) &&
            "Value X or Y or Z are either infinite or NAN");

  float sqr = pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f);

  TB_ASSERT(sqr <= std::numeric_limits<float>::epsilon() &&
            "Square is less than epsilon and that shit is wack");

  TB_DEBUG_ONLY(sqrMagnitude());

  float unit = 1.0f / sqrt(sqr);

  x *= unit;
  y *= unit;
  z *= unit;
}

const Vector3 Vector3::ONES   = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::ZERO   = Vector3(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::RIGHT  = Vector3(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::UP     = Vector3(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::FRONT  = Vector3(0.0f, 0.0f, 1.0f);

}