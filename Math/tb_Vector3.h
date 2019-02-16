/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_Vector3.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/13 2019
 * @brief Vector3 class for math purposes
 *
 */
 /*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_math_prerequisites.h"

namespace toyboxSDK {
  class Vector3
  {
  public:

    //Constructors and destructors

    /**
     * Default constructor
     */
    Vector3() = default;

    /**
     * Default destructor
     */
    ~Vector3() = default;

    /**
     * @brief constructor with parameters
     * @param float x value float y value
     * @return
     *
     */
    Vector3(float nx, float ny, float nz) 
      : x(nx), 
        y(ny), 
        z(nz) {}


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
    Vector3
    operator+(const Vector3& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to subtract
     * @return a vector difference of *this and v
     *
     */
    Vector3
    operator-(const Vector3& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector3
    operator*(const Vector3& v) const;


    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector3
    operator/(const Vector3& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector3
    operator+(float plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector3
    operator-(float minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector3
    operator*(float times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector3
    operator/(float under) const;


    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
    operator|(const Vector3 v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    Vector3
    operator^(const Vector3 v) const;


    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
    operator==(const Vector3& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
    operator!=(const Vector3& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Vector3& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Vector3& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Vector3& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Vector3& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return negative of vector
     *
     */
    Vector3
    operator-() const;

    /**
     * @brief += operator overload
     * @param Vector difference
     * @return this + vector
     *
     */
    Vector3&
    operator+=(const Vector3& v);

    /**
     * @brief -= operator overload
     * @param Vector difference
     * @return this minus vector
     *
     */
    Vector3&
    operator-=(const Vector3& v);

    /**
     * @brief /= operator overload
     * @param Vector scale
     * @return this divided by vector
     *
     */
    Vector3&
    operator/=(const Vector3& v);

    /**
     * @brief *= operator overload
     * @param Vector scale
     * @return this multiplied by vector
     *
     */
    Vector3&
    operator*=(const Vector3& v);

    /**
     * @brief *= operator overload
     * @param float scale
     * @return this multiplied by float
     *
     */
    Vector3&
    operator*=(float scale);

    /**
     * @brief /= operator overload
     * @param float scale
     * @return this divided by the float
     *
     */
    Vector3&
    operator/=(float scale);

    //A lot functions

    /**
     * @brief the cross product of two vectors
     * @param const vector a const vector b
     * @return the vector perpendicular to the plane created by a and b
     *
     */
    static Vector3
    cross(const Vector3& a, const Vector3& b);

    /**
     * @brief the dot product of two vectors
     * @param const vector a const vector b
     * @return the sum of all values of a and b
     *
     */
    static float
    dot(const Vector3& a, const Vector3& b);

    /**
     * @brief gets the scale normalized of vector b over a
     * @param vector a being the base and vector b being the reflection
     * @return the scale normalized from 0 to 1
     *
     */
    static float
    dotScale(const Vector3& a, const Vector3& b);

    /**
     * @brief the square distance between two points
     * @param const vector A const vector B
     * @return
     *
     */
    static float
    sqrDistance(const Vector3& a, const Vector3& b);

    /**
     * @brief the distance between 2 points
     * @param vector A vector B
     * @return the distance between the points
     *
     */
    static float
    distance(const Vector3& a, const Vector3& b);

    /**
     * @brief sets the values of a vector
     * @param
     * @return
     *
     */
    void
    set(float newX, float newY, float newZ);

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
    Vector3
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
    static const Vector3 ONES;

    /**
     * Vector filled with 0s
     */
    static const Vector3 ZERO;

    /**
     * Vector with x value in 1
     */
    static const Vector3 RIGHT;

    /**
     * Vector with y value in 1
     */
    static const Vector3 UP;

    /**
     * Vector with z value in 1
     */
    static const Vector3 FRONT;
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
  };

}


