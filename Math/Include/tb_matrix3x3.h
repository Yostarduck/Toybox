#pragma once

#include "tb_math_prerequisites.h"
#include "tb_math.h"
#include "tb_vector3.h"

namespace toyboxSDK {

class Matrix4x4;

 /**
 * Matrix with 3x3 elements
 *
 * Sample usage:
 * Matrix3x3 matrixName;
 */
class TB_MATH_EXPORT Matrix3x3                                                                 
{
 public:
  /**
  * Default constructor
  */
  Matrix3x3();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  explicit Matrix3x3(Math::FORCE_INIT k);

  /**
  * Constructor for matrix4x4
  *
  * Values are initialized with values for matrix4x4 
  * vector0, vector1 and vector2 values x,y,z 
  *
  */
  explicit Matrix3x3(const Matrix4x4& M);

  /**
  * Constructor for quaternion.
  */
  //explicit Matrix3x3(const Quaternion& Q);

  /**
  * Move constructor
  */
  Matrix3x3(Matrix3x3&& M) = default;

  /**
  * Copy constructor.
  */
  Matrix3x3(const Matrix3x3& M);

  /**
  * Initialize constructor with values.
  *
  * @param v0x, v0y, v0z
  *  The x, y, z values of the vector, first raw
  *
  * @param v1x, v1y, v1z
  *  The x, y, z values of the vector, second raw
  *
  * @param v2x, v2y, v2z
  *  The x, y, z values of the vector, third raw
  */
  Matrix3x3(float v0x, float v0y, float v0z,
            float v1x, float v1y, float v1z,
            float v2x, float v2y, float v2z);

  /**
  * Initialize constructor with values.
  *
  * @param vector0
  *  The vector0(x, y, z) values of the vector, first row
  *
  * @param vector10-vector12
  *  The vector1(x, y, z) values of the vector, second row
  *
  * @param vector20-vector22
  *  The vector2(x, y, z) values of the vector, third row
  */
  Matrix3x3(Vector3 vector0, Vector3 vector1, Vector3 vector2);

  /**
  * TEST::destructor
  * Default destructor
  *
  */
  ~Matrix3x3();

  /**
  * Calculates the determinant of the matrix
  *
  * @return
  *   determinant.
  */
  float
  determinant() const;

  /**
  * Calculates the cofactor of the matrix
  *
  * @return
  *   cofactor of matrix.
  */
  Matrix3x3&
  cofactor();

  /**
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix 
  * 
  * @return
  *   adjugate.
  */
  Matrix3x3&
  adjugate();

  /**
  * Calculate inverse of matrix.
  * using det * adjugate
  */
  Matrix3x3&
  inverse();

  /**
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 |T    | A1 | B1 | C1 |
  * | B1 | B2 | B3 |  =  | A2 | B2 | C2 |
  * | C1 | C2 | C3 |     | A3 | B3 | C3 |
  */
  Matrix3x3&
  transpose();

  /**
  * Identity
  *
  * Tranform matrix to identity
  */
  Matrix3x3&
  identity();

  /**
  * Check's if the other matrix is equal to this matrix with an error
  * range.
  *
  * @param otherMatrix
  *   Matrix to compare with this matrix.
  *
  * @param errorRange
  *   The value of the error range.
  *
  * @return
  *   bool expression of condition.
  */
  bool
  equals(const Matrix3x3& otherMatrix, float errorRange = Math::SMALL_NUMBER) const;

  /**
  * Euler angles form quaternion.
  *
  * @return
  *   vextor3d to values of angles.
  */
  Vector3
  eulerAngles();

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
  * 
  */
  Vector3&
  operator[](SizeT index);

  /**
  * 
  */
  Matrix3x3&
  operator=(const Matrix3x3& A);

  /**
  * 
  */
  Matrix3x3&
  operator=(const Matrix4x4& A);

  /**
  * 
  */
  Matrix3x3
  operator+(const Matrix3x3& A) const;

  /**
  * 
  */
  Matrix3x3&
  operator+=(const Matrix3x3& A);

  /**
  * 
  */
  Matrix3x3
  operator-(const Matrix3x3& A) const;

  /**
  * 
  */
  Matrix3x3&
  operator-=(const Matrix3x3& A);

  /**
  * 
  */
  Matrix3x3
  operator*(const Matrix3x3& A) const;

  /**
  * 
  */
  Matrix3x3&
  operator*=(const Matrix3x3& A);

  /**
  * 
  */
  Vector3
  operator*(const Vector3& S) const;

  /**
  * 
  */
  Matrix3x3
  operator*(const float S) const;

  /**
  * 
  */
  Matrix3x3&
  operator*=(const float S);

  /**
  * 
  */
  Matrix3x3
  operator/(const Matrix3x3& M) const;

  /**
  * 
  */
  Matrix3x3&
  operator/=(const Matrix3x3& M);

  /**
  * 
  */
  bool
  operator==(const Matrix3x3& M);

  /**
  * 
  */
  bool
  operator!=(const Matrix3x3& M);

  union
  {
    struct
    {
      Vector3 vector0, vector1, vector2;
    };
    Vector3 data[3];
  };

  static Matrix3x3 identityMat3x3;
  static Matrix3x3 zerosMat3x3;
};

}