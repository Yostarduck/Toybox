/*||偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|*/
/**
 * @file nauMatrix3.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/06 2019
 * @brief Matrix3x3 object class
 *
 */
 /*||偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|*/
#pragma once
#include "tb_math_prerequisites.h"
#include "tb_Vector2.h"
#include "tb_Vector3.h"

namespace toyboxSDK {

/**
 * Enum for static specific matrices
 */

enum MAT3 {
  kZERO,
  kONE
};


/**
 * Matrix3x3
 * Description:
 * 	A matrix 3by 3 for easier operations
 * Sample usage:
 *
 */
class Matrix3x3
{
public:

  /**
   * Default Constructor
   */
  Matrix3x3() = default;

  /**
   * Default Destructor
   */
  ~Matrix3x3() = default;

  /**
   * @brief Constructor with value parameter
   * @param value will be copied into all the matrix
   * @return
   */
  explicit Matrix3x3(Int32 value);

  /**
   * @brief Constructor that copies the info of another matrix
   * @param the other matrix to copy info from
   * @return
   *
   */
  Matrix3x3(const Matrix3x3& other);

  /**
   * @brief transforms the matrix into the IDENTITY matrix
   * @param
   * @return
   *
   */
  void
  identity();

  /**
   * @brief transforms the matrix into the ZERO matrix
   * @param
   * @return
   *
   */
  void
  zero();

  /**
   * @brief transposes the matrix
   * @param
   * @return
   *
   */
  void
  transposed();

  /**
   * @brief Sets the value of all the matrix to the parameter
   * @param the value which will be copied to all the matrix
   * @return
   *
   */
  void
  setValues(float value);

  /**
   * @brief gets the inverse of the matrix (if possible)
   * @param
   * @return
   *
   */
  void
  inverse();

  /**
   * @brief gets the determinant of the matrix
   * @param
   * @return the determinant
   *
   */
  float
  determinant();

  /**
   * @brief + operator overload
   * @param b the other matrix to add
   * @return the sum of *this and b
   *
   */
  FORCEINLINE Matrix3x3
  operator+(const Matrix3x3& b);

  /**
   * @brief - operator overload
   * @param b the other matrix to subtract
   * @return the difference of *this and b
   *
   */
  FORCEINLINE Matrix3x3
  operator-(const Matrix3x3& b);

  /**
   * @brief * operator overload
   * @param b the other matrix to multiply
   * @return the multiplication of *this and b
   *
   */
  FORCEINLINE Matrix3x3
  operator*(const Matrix3x3& b);

  /**
   * @brief += operator overload
   * @param b the other matrix to add
   * @return *this summed b
   *
   */
  Matrix3x3&
  operator+=(const Matrix3x3& b);

  /**
   * @brief -= operator overload
   * @param b the other matrix to subtract
   * @return *this minus b
   *
   */
  Matrix3x3&
  operator-=(const Matrix3x3& b);

  /**
   * @brief *= operator overload
   * @param b the other matrix to multiply
   * @return *this summed b
   *
   */
  Matrix3x3&
  operator*=(const Matrix3x3& b);

  /**
   * @brief += operator overload
   * @param value to add to the matrix
   * @return naumatrix equal to *this summed value
   *
   */
  Matrix3x3&
  operator+=(const float& value);

  /**
   * @brief -= operator overload
   * @param value to subtract to the matrix
   * @return naumatrix equal to *this minus value
   *
   */
  Matrix3x3&
  operator-=(const float& value);

  /**
   * @brief *= operator overload
   * @param value to multiply the matrix
   * @return naumatrix equal to *this times value
   *
   */
  Matrix3x3&
  operator*=(const float& value);

  /**
   * @brief /= operator overload
   * @param value to divide to the matrix
   * @return naumatrix equal to *this folded value
   *
   */
  Matrix3x3&
  operator/=(const float& value);

  /**
   * @brief == operator overload
   * @param nauMatrix to compare
   * @return true if *this is equal to b
   *
   */
  bool
  operator==(const Matrix3x3& b);


  /**
   * Member declaration
   */
 public:

  /**
   * Matrix component
   */
  union {
    /**
     * Matrix struct made by 4x4 array column major
     */
    struct {
      float m00, m10, m20;
      float m01, m11, m21;
      float m02, m12, m22;
    } _m;
    float m[3][3];
    Vector3 vec[3];
    float fVec[9];
  };

  /**
   * ZERO filled Matrix
   */
  static const Matrix3x3 ZERO;

  /**
   * IDENTITY matrix
   */
  static const Matrix3x3 IDENTITY;

};

}