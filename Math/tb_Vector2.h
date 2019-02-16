/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_Vector2.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/13 2019
 * @brief Vector2 class for math purposes
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_math_prerequisites.h"

namespace toyboxSDK {
  class Vector2
  {
  public:

    //Constructors and destructors

    /**
     * Default constructor
     */
    Vector2() = default;

    /**
     * Default destructor
     */
    ~Vector2() = default;

    /**
     * @brief constructor with parameters
     * @param float x value float y value
     * @return 
     *
     */
    Vector2(float newX, float newY)
      : x(newX),
        y(newY) {}

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
    Vector2
    operator+(const Vector2& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to substract
     * @return a vector difference of *this and v
     *
     */
    Vector2
    operator-(const Vector2& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector2
    operator*(const Vector2& v) const;


    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector2
    operator/(const Vector2& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector2
    operator+(float plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector2
    operator-(float minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector2
    operator*(float times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector2
    operator/(float under) const;


    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
    operator|(const Vector2 v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    float
    operator^(const Vector2 v) const;


    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
    operator==(const Vector2& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
    operator!=(const Vector2& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Vector2& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Vector2& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Vector2& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Vector2& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return negative of vector
     *
     */
    Vector2
    operator-() const;

    /**
     * @brief += operator overload
     * @param Vector difference
     * @return this + vector
     *
     */
    Vector2&
    operator+=(const Vector2& v);

    /**
     * @brief -= operator overload
     * @param Vector difference
     * @return this minus vector
     *
     */
    Vector2&
    operator-=(const Vector2& v);

    /**
     * @brief /= operator overload
     * @param Vector scale
     * @return this divided by vector
     *
     */
    Vector2&
    operator/=(const Vector2& v);

    /**
     * @brief *= operator overload
     * @param Vector scale
     * @return this multiplied by vector
     *
     */
    Vector2&
    operator*=(const Vector2& v);

    /**
     * @brief *= operator overload
     * @param float scale
     * @return this multiplied by float
     *
     */
    Vector2&
    operator*=(float scale);

    /**
     * @brief /= operator overload
     * @param float scale
     * @return this divided by the float
     *
     */
    Vector2&
    operator/=(float scale);

    //A lot functions

    /**
     * @brief the cross product of two vectors
     * @param const vector a const vector b
     * @return the vector perpendicular to the plane created by a and b
     *
     */
    static float
    dot(const Vector2& a, const Vector2& b);

    /**
     * @brief gets the scale normalized of vector b over a
     * @param vector a being the base and vector b being the reflection
     * @return the scale normalized from 0 to 1
     *
     */
    static float
    dotScale(const Vector2& a, const Vector2& b);

    /**
     * @brief the square distance between two points
     * @param const vector A const vector B
     * @return 
     *
     */
    static float
    sqrDistance(const Vector2& a, const Vector2& b);

    /**
     * @brief the distance between 2 points
     * @param vector A vector B
     * @return the distance between the points
     *
     */
    static float
    distance(const Vector2& a, const Vector2& b);

    /**
     * @brief sets the values of a vector
     * @param 
     * @return 
     *
     */
    void
    set(float newX, float newY);

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
    Vector2
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
    static const Vector2 ONES;

    /**
     * Vector filled with 0s
     */
    static const Vector2 ZERO;

    /**
     * Vector with x value in 1
     */
    static const Vector2 RIGHT;

    /**
     * Vector with y value in 1
     */
    static const Vector2 UP;

  public:

    /**
     * X float value of the vector
     */
    float x;

    /**
     * Y float value of the vector
     */
    float y;
  };
  
}


