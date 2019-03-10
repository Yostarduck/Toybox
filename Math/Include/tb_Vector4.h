#pragma once

#include "tb_math_prerequisites.h"
#include "tb_math.h"

namespace toyboxSDK {

class Vector2;
class Vector3;
class Matrix4x4;

/**
* Vector with 4 elements (x, y, z, w)
*
* Sample usage:
* Vector3 vectorName;
*/
class TB_MATH_EXPORT Vector4
{
 public:
  /**
  * Default constructor.
  */
  Vector4();

  /**
  * Default constructor.
  *
  * @param k
  *
  * Values are initialized with 0.
  */
  explicit Vector4(Math::FORCE_INIT k);

  /**
  * Move constructor.
  */
  Vector4(Vector4&& V) = default;

  /**
  * Copy constructor.
  */
  Vector4(const Vector4& V);

  /**
  * Copy constructor for vec3 to vec4.
  */
  Vector4(const Vector3& V);

  /**
  * Copy constructor for vec3 to vec4 with w value.
  */
  Vector4(const Vector3& V, float _w);

  /**
  * Copy constructor for two vec2 to vec4.
  */
  Vector4(const Vector2& V1, const Vector2& V2);

  /**
  * Initialize constructor with values.
  *
  * @param _x
  *  The _x value of the vector
  *
  * @param _y
  *  The _y value of the vector
  *
  * @param _z
  *  The _z value of the vector
  *
  * @param _w
  *  The _w value of the vector
  */
  Vector4(float _x, float _y, float _z, float _w);

  /**
  * Default destructor
  */
  ~Vector4();

  /**
  * Computes the dot product (excluding w) between this vector and the vector parameter.
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
  dot3(const Vector4& B) const;

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
  dot(const Vector4& B) const;

  /**
  * Computes the cross product between this vector and the vector parameter.
  * W value is not used, and it's final value will be 0.
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
  Vector4
  cross(const Vector4& B) const;

  /**
  * Computes the length of this vector.
  *
  * @return
  *   The length (or "size") of the vector.
  */
  float
  length3() const;

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
  lengthSqr3() const;

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
  void
  normalize3();

  /**
  * Normalize the vector.
  */
  void
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
  distance3(const Vector4& otherVector) const;

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
  distance(const Vector4& otherVector) const;

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
  distanceSqr3(const Vector4& otherVector) const;

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
  distanceSqr(const Vector4& otherVector) const;

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
  equals(const Vector4& otherVector, float errorRange = Math::SMALL_NUMBER) const;

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
  * Gets a const reference to the specified element from the vector.
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
  operator|(const Vector4& B) const;

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
  Vector4
  operator^(const Vector4& B) const;

  /**
  * Set the x, y, z, w values of this vector
  * with the x, y, z, w of the other vector.
  *
  * @param A
  *  The vector with the values to use.
  *
  * @return
  *   A reference to this vector.
  */
  Vector4&
  operator=(const Vector4& A);

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
  Vector4&
  operator=(const Vector3& A);

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
  Vector4
  operator+(const Vector4& A) const;

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
  Vector4&
  operator+=(const Vector4& A);

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
  Vector4
  operator-(const Vector4& A) const;

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
  Vector4&
  operator-=(const Vector4& A);

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
  Vector4
  operator*(const Vector4& A) const;

  /**
  * Multiplies this vector with matrix.
  *
  * @param matrix
  *  The matrix to multiply with.
  *  Matrix is left param (transposed).
  *
  * @return
  *   A vector with the result values.
  */
  Vector4
  operator*(Matrix4x4 matrix) const;

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
  Vector4&
  operator*=(const Vector4& A);

  /**
  * Multiplies elements of the vector.
  *
  * @param scalar
  *  The factor.
  *
  * @return
  *   A vector with the result values.
  */
  Vector4
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
  Vector4&
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
  Vector4
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
  Vector4&
  operator/=(const float scalar);

  /**
  * Checks if the values of the vectors are equal.
  *
  * @return
  *   Flag if vectors are equal.
  */
  bool
  operator==(const Vector4& otherVector);

  /**
  * Checks if the values of the vectors aren't equal.
  *
  * @return
  *   Flag if vectors aren't equal.
  */
  bool
  operator!=(const Vector4& otherVector);

  /**
  * Returns this vector with negated values.
  */
  Vector4
  operator-() const;

  union
  {
    struct
    {
      float x, y, z, w;
    };
    float data[4];
  };
};

}