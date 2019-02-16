/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_Vector2.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/13 2019
 * @brief Vector implementation
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#include "tb_Vector2.h"

namespace toyboxSDK {
  using std::isnan;
  using std::isinf;
  using std::pow;
  using std::sqrt;

  float&
  Vector2::operator[](UInt32 index) {
    return (&x)[index];
  }

  float
  Vector2::operator[](UInt32 index) const {
    return (&x)[index];
  }

  Vector2
  Vector2::operator+(const Vector2& v) const {
    return Vector2(x + v.x, y + v.y);
  }

  Vector2
  Vector2::operator-(const Vector2& v) const {
    return Vector2(x - v.x, y - v.y);
  }

  Vector2
  Vector2::operator*(const Vector2& v) const {
    return Vector2(x * v.x, y * v.y);
  }

  Vector2
  Vector2::operator/(const Vector2& v) const {
    return Vector2(x / v.x, y / v.y);
  }

  Vector2
  Vector2::operator+(float plus) const {
    return Vector2(x + plus, y + plus);
  }

  Vector2
  Vector2::operator-(float minus) const {
    return Vector2(x - minus, y - minus);
  }
  Vector2
  Vector2::operator*(float times) const {
    return Vector2(x * times, y * times);
  }
  Vector2
  Vector2::operator/(float under) const {
    return Vector2(x / under, y / under);
  }
  float
  Vector2::operator|(const Vector2 v) const {
    return x * v.x + y * v.y;
  }
  
  float
  Vector2::operator^(const Vector2 v) const {
    return x * v.x - y * v.y;
  }

  bool
  Vector2::operator==(const Vector2& v) const {
    return x == v.x && y == v.y;
  }

  bool
  Vector2::operator!=(const Vector2& v) const {
    return x != v.x || y != v.y;
  }

  bool
  Vector2::operator<(const Vector2& v) const {
    return x < v.x && y < v.y;
  }
  
  bool
  Vector2::operator>(const Vector2& v) const {
    return x > v.x && y > v.y;
  }

  bool
  Vector2::operator<=(const Vector2& v) const {
    return x <= v.x && y <= v.y;
  }

  bool
  Vector2::operator>=(const Vector2& v) const {
    return x >= v.x && y >= v.y;
  }

  Vector2
  Vector2::operator-() const {
    return Vector2(-x, -y);
  }

  Vector2&
  Vector2::operator+=(const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  Vector2&
  Vector2::operator-=(const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  Vector2&
  Vector2::operator/=(const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  Vector2&
  Vector2::operator*=(const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  Vector2&
  Vector2::operator*=(float scale) {
    x *= scale;
    y *= scale;
    return *this;
  }

  Vector2&
  Vector2::operator/=(float scale) {
    x /= scale;
    y /= scale;
    return *this;
  }

  float
  Vector2::dot(const Vector2& a, const Vector2& b) {
    return a | b;
  }

  float
  Vector2::dotScale(const Vector2& a, const Vector2& b) {
    return (a | b) / a.magnitude();
  }

  float
  Vector2::sqrDistance(const Vector2& a, const Vector2& b) {
    return pow(a.x - b.x, 2.0f) + pow(a.y - b.y, 2.0f);
  }

  float
  Vector2::distance(const Vector2& a, const Vector2& b) {
    return sqrt(pow(a.x - b.x,2.0f) + pow(a.y - b.y, 2.0f));

  }

  void
  Vector2::set(float newX, float newY) {
    x = newX;
    y = newY;
  }

  void
  Vector2::round() {
    x = std::round(x);
    y = std::round(y);
  }

  float
  Vector2::magnitude() const {
    return sqrt(x * x + y * y);
  }

  float
  Vector2::sqrMagnitude() const {
    return (x * x + y * y);
  }

  Vector2
  Vector2::normalized() const {
    
    TB_ASSERT(!isnan(x) &&
              !isnan(y) &&
              !isinf(x) &&
              !isinf(y) &&
              "Value X or Y are either infinite or NAN");

    float sqr = pow(x, 2.0f) + pow(y, 2.0f);

    TB_ASSERT(sqr <= std::numeric_limits<float>::epsilon() &&
      "Square is less than epsilon and that shit is wack");

    TB_DEBUG_ONLY(sqrMagnitude());

    float unit = 1.0f / sqrt(sqr);
    return Vector2((x * unit), (y * unit));
  }

  void
  Vector2::normalize() {
    TB_ASSERT(!isnan(x) &&
      !isnan(y) &&
      !isinf(x) &&
      !isinf(y) &&
      "Value X or Y are either infinite or NAN");

    float sqr = pow(x, 2.0f) + pow(y, 2.0f);

    TB_ASSERT(sqr <= std::numeric_limits<float>::epsilon() &&
      "Square is less than epsilon and that shit is wack");

    TB_DEBUG_ONLY(sqrMagnitude());

    float unit = 1.0f / sqrt(sqr);

    x *= unit;
    y *= unit;
  }

  const Vector2 Vector2::ONES   = Vector2(1.0f, 1.0f);

  const Vector2 Vector2::ZERO   = Vector2(0.0f, 0.0f);

  const Vector2 Vector2::RIGHT  = Vector2(1.0f, 0.0f);

  const Vector2 Vector2::UP     = Vector2(0.0f, 1.0f);
}
