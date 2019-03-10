#pragma once

#include "tb_math_prerequisites.h"
#include "tb_math.h"
#include "tb_Vector3.h"
#include "tb_Vector4.h"

namespace toyboxSDK {

class Matrix3x3;

/**
* Matrix with 4x4 elements
*
* Sample usage:
*   Matrix4x4 matrixName;
*/
class TB_MATH_EXPORT Matrix4x4
{
 public:
  /**
  * Default constructor
  *
  */
  Matrix4x4();

  /**
  * Default constructor
  *
  * Values are initialized with 0(kZero) or identity matrix(kIdentity).
  */
  explicit Matrix4x4(Math::FORCE_INIT k);

  /**
  * Constructor for matrix3x3.
  *
  * Values are initialized with values for matrix3x3
  * vector0, vector1 and vector2 
  * values of w initialize in 0 except vector4.w in 1
  *
  */
  explicit Matrix4x4(const Matrix3x3& M);

  /**
  * Constructor for quaternion
  * values of w intitalize in 0 and vector4.w in 1
  *
  */
  //explicit Matrix4x4(const Quaternion& Q);

  /**
  * Move constructor.
  */
  Matrix4x4(Matrix4x4&& M) = default;

  /**
  * Copy constructor.
  */
  Matrix4x4(const Matrix4x4& M);

  /**
  * Initialize constructor with values.
  *
  * @param v0x, v0y, v0z, v0w
  *  The x, y, z, w values of the vector, first row
  *
  * @param v1x, v1y, v1z, v1w
  *  The x, y, z, w values of the vector, second row
  *
  * @param v2x, v2y, v2z, v2w
  *  The x, y, z, w values of the vector, third row
  *
  * @param v3x, v3y, v3z, v3w
  *  The x, y, z, w values of the vector, quarter row
  *
  */
  Matrix4x4(float v0x, float v0y, float v0z, float v0w,
            float v1x, float v1y, float v1z, float v1w,
            float v2x, float v2y, float v2z, float v2w,
            float v3x, float v3y, float v3z, float v3w);

  /**
  * Initialize constructor with values.
  *
  * @param vector0
  *  The vector0(x, y, z, w) values of the vector, first raw
  *
  * @param vector1
  *  The vector1(x, y, z, w) values of the vector, second raw
  *
  * @param vector3
  *  The vector2(x, y, z, w) values of the vector, third raw
  *
  * @param vector4
  *  The vector2(x, y, z, w) values of the vector, quarter raw
  */
  Matrix4x4(Vector4 vector0, Vector4 vector1, Vector4 vector2, Vector4 vector3);

  /**
  * Destructor
  *
  * Default destructor
  *
  */
  ~Matrix4x4();

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
  Matrix4x4&
  cofactor();

  /**
  * Calculates the adjugate of the matrix
  * using transpose for cofactor matrix
  *
  * @return
  *   adjugate.
  */
  Matrix4x4&
  adjugate();

  /**
  * Calculate inverse of matrix.
  * using det * adjugate
  */
  Matrix4x4&
  inverse();

  /**
  * Transpose
  *
  * Invert columns whith rows
  * | A1 | A2 | A3 | A4 |T    | A1 | B1 | C1 | D1 |
  * | B1 | B2 | B3 | B4 |     | A2 | B2 | C2 | D2 |
  * | C1 | C2 | C3 | C4 |  =  | A3 | B3 | C3 | D3 |
  * | D1 | D2 | D3 | C4 |     | A4 | B4 | C4 | D4 |
  */
  Matrix4x4&
  transpose();

  /**
  * Identity
  *
  * Tranform matrix to identity
  */
  Matrix4x4&
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
  equals(const Matrix4x4& otherMatrix, float errorRange = Math::SMALL_NUMBER) const;

  /**
  * Euler angles form quaternion.
  *
  * @return
  *   vextor3d to values of angles.
  */
  Vector3
  eulerAngles() const;

  /**
  * 
  */
  //Quaternion
  //toQuaternion();

  /**
  * Translate
  *
  * Change position to vector
  */
  Matrix4x4&
  Translation(const Vector3 &Pos);

  /**
  * Move
  *
  * Add position to vector
  */
  Matrix4x4&
  Move(const Vector3 &Move);

  /**
  * Scale
  *
  * Change scale of matrix
  */
  Matrix4x4&
  Scale(const Vector3 &Scale);

  /**
  *
  */
  Matrix4x4
  InitScale(const Vector3 &Scale);

  /**
  * Rotation
  *
  * Aplication rotation z, x, y
  */
  Matrix4x4&
  Rotation(const float Yaw, const float Pitch, const float Roll);

  
  /**
  * Aplies a XYZ rotation.
  *
  * @param angles
  *   The x, y, z angles.
  *
  * @return
  *   The resulting rotation matrix.
  */
  Matrix4x4&
  Rotation(const Vector3& u, float thetaRadians);

  /**
  * Rotation
  *
  * Aplication rotation x
  */
  Matrix4x4&
  RotationX(const float teta);

  /**
  * RotationY
  *
  * Aplication rotation Y
  */
  Matrix4x4&
  RotationY(const float teta);

  /**
  * RotationZ
  *
  * Aplication rotation Z
  */
  Matrix4x4&
  RotationZ(const float teta);

  /**
  */
  Matrix4x4&
  LookAt(const Vector3 &Eye, const Vector3 &At, const Vector3 &Up);

  /**
  * Matrix for look at for camera
  */
  Matrix4x4&
  Projection(float Width, float Height, float ZNear, float ZFar);

  /**
  */
  Matrix4x4&
  ProjectionFov(float FOV, float Aspect, float ZNear, float ZFar);

  /**
  */
  Matrix4x4&
  Orthogonal(float Width, float Height, float ZNear, float ZFar);

  /**
  */
  Matrix4x4&
  Reflection(Vector3 NormalOfMirror);

  /*
  Vector3
  eulerAngles();
  */
  
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
  Vector4&
  operator[](SizeT index);

  /**
  * 
  */
  Matrix4x4&
  operator=(const Matrix4x4& A);

  /**
  * w value not modified or asigned
  */
  Matrix4x4&
  operator=(const Matrix3x3& A);

  /**
  * 
  */
  Matrix4x4
  operator+(const Matrix4x4& A) const;

  /**
  * 
  */
  Matrix4x4&
  operator+=(const Matrix4x4& A);

  /**
  * 
  */
  Matrix4x4
  operator-(const Matrix4x4& A) const;

  /**
  * 
  */
  Matrix4x4&
  operator-=(const Matrix4x4& A);

  /**
  * 
  */
  Matrix4x4
  operator*(const Matrix4x4& A) const;

  /**
  * 
  */
  Matrix4x4&
  operator*=(const Matrix4x4& A);

  /**
  * 
  */
  Vector4
  operator*(const Vector4& S) const;

  /**
  * 
  */
  Matrix4x4
  operator*(const float S) const;

  /**
  * 
  */
  Matrix4x4&
  operator*=(const float S);

  /**
  * 
  */
  Matrix4x4
  operator/(const Matrix4x4& M) const;

  /**
  * 
  */
  Matrix4x4&
  operator/=(const Matrix4x4& M);

  /**
  * 
  */
  bool
  operator==(const Matrix4x4& M);

  /**
  * 
  */
  bool
  operator!=(const Matrix4x4& M);

  union
  {
    struct
    {
      Vector4 vector0, vector1, vector2, vector3;
    };
    Vector4 data[4];
  };

  static Matrix4x4 identityMat4x4;
  static Matrix4x4 zerosMat4x4;
};

}