/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_Vector4.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/13 2019
 * @brief Vector4 class for math purposes
 *
 */
 /*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_math_prerequisites.h"
#include "tb_Vector3.h"

namespace toyboxSDK {
  class Vector4
  {
  public:

    //Constructors and destructors

    /**
     * Default constructor
     */
    Vector4() = default;

    /**
     * Vector3 Constructor
     */
    Vector4(Vector3 og);

    /**
     * Default destructor
     */
    ~Vector4() = default;

    /**
     * @brief constructor with parameters
     * @param float x value float y value
     * @return
     *
     */
    Vector4(float newX, float newY, float newZ, float newW = 1.0f)
      : x(newX),
        y(newY),
        z(newZ),
        w(newW) {}

    //A lot of operators

    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    float&
      operator[](UInt32 index);

    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    float
      operator[](UInt32 index) const;

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return a vector sum of *this and v
     *
     */
    Vector4
      operator+(const Vector4& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to substract
     * @return a vector difference of *this and v
     *
     */
    Vector4
      operator-(const Vector4& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector4
      operator*(const Vector4& v) const;


    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector4
      operator/(const Vector4& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector4
      operator+(float plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector4
      operator-(float minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector4
      operator*(float times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector4
      operator/(float under) const;


    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
      operator|(const Vector4 v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    Vector4
      operator^(const Vector4 v) const;


    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
      operator==(const Vector4& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
      operator!=(const Vector4& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
      operator<(const Vector4& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
      operator>(const Vector4& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
      operator<=(const Vector4& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
      operator>=(const Vector4& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return negative of vector
     *
     */
    Vector4
      operator-() const;

    /**
     * @brief += operator overload
     * @param Vector difference
     * @return this + vector
     *
     */
    Vector4&
      operator+=(const Vector4& v);

    /**
     * @brief -= operator overload
     * @param Vector difference
     * @return this minus vector
     *
     */
    Vector4&
      operator-=(const Vector4& v);

    /**
     * @brief /= operator overload
     * @param Vector scale
     * @return this divided by vector
     *
     */
    Vector4&
      operator/=(const Vector4& v);

    /**
     * @brief *= operator overload
     * @param Vector scale
     * @return this multiplied by vector
     *
     */
    Vector4&
      operator*=(const Vector4& v);

    /**
     * @brief *= operator overload
     * @param float scale
     * @return this multiplied by float
     *
     */
    Vector4&
      operator*=(float scale);

    /**
     * @brief /= operator overload
     * @param float scale
     * @return this divided by the float
     *
     */
    Vector4&
      operator/=(float scale);

    //A lot functions

    /**
     * @brief the cross product of two vectors
     * @param const vector a const vector b
     * @return the vector perpendicular to the plane created by a and b
     *
     */
    static Vector4
      cross(const Vector4& a, const Vector4& b);

    /**
     * @brief the dot product of two vectors
     * @param const vector a const vector b
     * @return the sum of all values of a and b
     *
     */
    static float
      dot(const Vector4& a, const Vector4& b);

    /**
     * @brief gets the scale normalized of vector b over a
     * @param vector a being the base and vector b being the reflection
     * @return the scale normalized from 0 to 1
     *
     */
    static float
      dotScale(const Vector4& a, const Vector4& b);

    /**
     * @brief the square distance between two points
     * @param const vector A const vector B
     * @return
     *
     */
    static float
      sqrDistance(const Vector4& a, const Vector4& b);

    /**
     * @brief the distance between 2 points
     * @param vector A vector B
     * @return the distance between the points
     *
     */
    static float
      distance(const Vector4& a, const Vector4& b);

    /**
     * @brief sets the values of a vector
     * @param
     * @return
     *
     */
    void
      set(float newX, float newY, float newZ, float newW);

    /**
     * @brief rounds the values to the nearest integer
     * @param
     * @return
     *
     */
    void
      round();

    /**
     * @brief squares the values in the vector and gets the square root
     * @param
     * @return the square root of the components squared
     *
     */
    float
      magnitude() const;

    /**
     * @brief squares the values in the vector and sums it
     * @param
     * @return the square sum of the components
     *
     */
    float
      sqrMagnitude() const;

    /**
     * @brief creates the normalized vector
     * @param
     * @return returns the normalized vector
     *
     */
    Vector4
      normalized() const;

    /**
     * @brief normalizes the vector
     * @param
     * @return
     *
     */
    void
      normalize();

    //A lot of constants

    /**
     * Vector filled with 1s
     */
    static const Vector4 ONES;

    /**
     * Vector filled with 0s
     */
    static const Vector4 ZERO;

    /**
     * Vector with x value in 1
     */
    static const Vector4 RIGHT;

    /**
     * Vector with y value in 1
     */
    static const Vector4 UP;

    /**
     * Vector with z value in 1
     */
    static const Vector4 FRONT;
  public:

    /**
     * X float value of the vector
     */
    float x;

    /**
     * Y float value of the vector
     */
    float y;

    /**
     * Z float value of the vector
     */
    float z;

    /**
     * W float value of the vector
     */
    float w;
  };

}


