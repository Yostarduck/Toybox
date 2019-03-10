#pragma once

#include "tb_math_prerequisites.h"
#include "tb_math.h"

namespace toyboxSDK {

class Vector4;

/**
* Vector with 3 elements (x, y, z)
*
* Sample usage:
* Vector3 vectorName;
*/
class TB_MATH_EXPORT Vector3
{
 public:
  /**
  * Default constructor.
  */
  Vector3() {}

  /**
  * Default constructor.
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector3(Math::FORCE_INIT k);

  /**
  * Move constructor.
  */
  Vector3(Vector3&& V) = default;

  /**
  * Copy constructor.
  */
  Vector3(const Vector3& V);

  /**
  * Copy constructor for Vec4 to Vec3.
  */
  explicit Vector3(const Vector4& V);

  /**
  * Initialize constructor with values.
  *
  * @param _x
  *  The x value of the vector
  *
  * @param _y
  *  The y value of the vector
  *
  * @param _z
  *  The z value of the vector
  */
  Vector3(float _x, float _y, float _z);

  /**
  * Default destructor.
  */
  ~Vector3();

  /**
  * Computes the dot product between this vector and the vector parameter.
  * This operatios is commutative.
  *
  * @param B
  *  The vector against which the dot product is calculated.
  *
  * @return
  *   The sum of the products of the corresponding entries of the
  *   vectors.
  */
  float
  dot(const Vector3& B) const;

  /**
  * Computes the cross product between this vector and the vector parameter.
  * This operatios is NOT commutative.
  *
  * @param B
  *  The vector against which the cross product is calculated.
  *  B (vector parameter) is the rigth value of operation
  *  AxB
  *
  * @return
  *   Result vector of the cross product.
  */
  Vector3
  cross(const Vector3& B) const;

  /**
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length() const;

  /**
  * Computes the squared length of this vector.
  *
  * @return
  *   The length (or "size") of the vector squared.
  */
  float
  lengthSqr() const;

  /**
  * Normalize the vector.
  */
  Vector3&
  normalize();

  /**
  * Computes the distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distance(const Vector3& otherVector) const;

  /**
  * Computes the squared distance between two vectors.
  *
  * @param scalar
  *   Vector to calculate the distance
  *
  * @return
  *   Distance
  */
  float
  distanceSqr(const Vector3& otherVector) const;

  /**
  * Check's if the other vector is equal to this vector with an error
  * range.
  *
  * @param otherVector
  *   Vector to compare with this vector.
  *
  * @param errorRange
  *   The value of the error range.
  *
  * @return
  *   bool expression of condition.
  */
  bool
  equals(const Vector3& otherVector, float errorRange = Math::SMALL_NUMBER) const;

  /**
  * Computes the squared distance between a point and a segment.
  *
  * @param pointA
  *   Point a of the segment.
  *
  * @param pointB
  *   Point b of the segment.
  *
  * @return
  *   Distance
  */
  float
  sqrDistSegment(const Vector3& pointA, const Vector3& pointB) const;

  /**
  * Gets a pointer to the first element of the matrix.
  *
  * @return
  *	  A pointer to the first element of the matrix.
  */
  float*
  ptr();

  /**
  * Gets a constant pointer to the first element of the matrix.
  *
  * @return
  *	  A constant pointer to the first element of the matrix.
  */
  const float*
  ptr() const;

  /**
  * Gets a reference to the specified element from the vector.
  *
  *	@param index
  *	 The index of the element.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */
  float&
  operator[](const SizeT index);

  /**
  * Gets a reference to the specified element from the vector.
  *
  *	@param index
  *	 The index of the element.
  *
  * @return
  *	  A const reference to the element at the [index] position.
  *
  * @throws out_of_range
  *	  If the index is greater than number of elements in the vector.
  */
  const float&
  operator[](const SizeT index) const;

  /**
  * Computes the dot product between this vector and the vector parameter.
  * This operatios is commutative.
  *
  * @param B
  *  The vector against which the dot product is calculated.
  *
  * @return
  *   The sum of the products of the corresponding entries of the
  *   vectors.
  */
  float
  operator|(const Vector3& B) const;

  /**
  * Computes the cross product between this vector and the vector parameter.
  * This operatios is NOT commutative.
  *
  * @param B
  *  The vector against which the cross product is calculated.
  *  B (vector parameter) is the rigth value of operation
  *  AxB
  *
  * @return
  *   Result vector of the cross product
  */
  Vector3
  operator^(const Vector3& B) const;

  /**
  * Set the x, y, z values of this vector
  * with the x, y, z of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator=(const Vector3& A);

  /**
  * Set the x, y, z values of this vector
  * with the x, y, z of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator=(const Vector4& A);

  /**
  * Adds elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3
  operator+(const Vector3& A) const;

  /**
  * Adds elements of the vector with
  * the elements of the other vector
  * and stores it in this vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator+=(const Vector3& A);

  /**
  * Substracts elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3
  operator-(const Vector3& A) const;

  /**
  * Substracts elements of the vector with
  * the elements of the other vector and
  * stores it in this vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator-=(const Vector3& A);

  /**
  * Multiplies elements of the vector with
  * the elements of the other vector.
  *
  * @param A
  *  The vector with the factors to use.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3
  operator*(const Vector3& A) const;

  /**
  * Multiplies elements of the vector with
  * the elements of the other vector, and stores
  * the result in this vector.
  *
  * @param A
  *  The vector with the factors to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator*=(const Vector3& A);

  /**
  * Multiplies elements of the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3
  operator*(const float scalar) const;

  /**
  * Multiplies elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator*=(const float scalar);

  /**
  * Divide elements of the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A vector with the result values.
  */
  Vector3
  operator/(const float scalar) const;

  /**
  * Divide elements of the vector and stores it in the vector.
  *
  * @param scalar
  *  The dividend.
  *
  * @return
  *   A reference to this vector.
  */
  Vector3&
  operator/=(const float scalar);

  /**
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector3& otherVector) const;

  /**
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector3& otherVector) const;

  /**
  * Returns this vector with negated values.
  */
  Vector3
  operator-() const;

  union
  {
    struct
    {
      float x, y, z;
    };
    float data[3];
  };
};

}