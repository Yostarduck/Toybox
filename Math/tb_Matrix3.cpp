/*||偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|*/
/**
 * @file nauMatrix3.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/06 2019
 * @brief Matrix3 implementation
 *
 */
 /*||偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|*/
#include "tb_Matrix3.h"

namespace toyboxSDK {

  const Matrix3 Matrix3::ZERO = Matrix3(0);

  const Matrix3 Matrix3::IDENTITY = Matrix3(MAT3::ONE);

  Matrix3::Matrix3(const Matrix3& other) {
    _m = other._m;
  }

  Matrix3::Matrix3(Int32 value) {
    memset(this, 0, sizeof(Matrix3));

    if (MAT3::ZERO != value) {
      _m.m00 = _m.m11 = _m.m22 = 1.0f;
    }
  }

  void
    Matrix3::identity() {
    *this = IDENTITY;
  }

  void
    Matrix3::zero() {
    *this = ZERO;
  }

  void
    Matrix3::transposed() {
    Matrix3 temp = *this;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        m[i][j] = temp.m[j][i];
      }
    }
  }

  void
    Matrix3::setValues(float value) {
    memset(&_m, static_cast<int>(value), sizeof(_m));
  }

  void
    Matrix3::inverse() {
    float det = determinant();
    if (det == 0.0f) {
      printf("Matrix cant be inverse");
      return;
    }
    Matrix3 temp = *this;
    m[0][0] = ((temp.m[1][1] * temp.m[2][2]) - (temp.m[1][2] * temp.m[2][1]));
    m[1][0] = -((temp.m[1][0] * temp.m[2][2]) - (temp.m[1][2] * temp.m[2][0]));
    m[2][0] = ((temp.m[1][0] * temp.m[2][1]) - (temp.m[1][1] * temp.m[2][0]));
    m[0][1] = -((temp.m[0][1] * temp.m[2][2]) - (temp.m[0][2] * temp.m[1][2]));
    m[1][1] = ((temp.m[0][0] * temp.m[2][2]) - (temp.m[0][2] * temp.m[2][0]));
    m[2][1] = -((temp.m[0][0] * temp.m[2][1]) - (temp.m[0][1] * temp.m[2][0]));
    m[0][2] = ((temp.m[0][1] * temp.m[1][2]) - (temp.m[0][2] * temp.m[1][1]));
    m[1][2] = -((temp.m[0][0] * temp.m[1][2]) - (temp.m[0][2] * temp.m[1][0]));
    m[2][2] = ((temp.m[0][0] * temp.m[1][1]) - (temp.m[0][1] * temp.m[1][0]));

    *this /= det;
  }

  float
    Matrix3::determinant() {
    return (((m[0][0] * m[1][1] * m[2][2]) +
      (m[1][0] * m[2][1] * m[0][2]) +
      (m[2][0] * m[0][1] * m[1][2])) -

      ((m[0][2] * m[1][1] * m[2][0]) +
      (m[1][2] * m[2][1] * m[0][0]) +
        (m[2][2] * m[0][1] * m[1][0])));
  }

  FORCEINLINE Matrix3
    Matrix3::operator+(const Matrix3& b) {
    Matrix3 temp;
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        temp.m[i][j] = m[i][j] + b.m[i][j];
      }
    }
    return temp;
  }

  FORCEINLINE Matrix3
    Matrix3::operator-(const Matrix3& b) {
    Matrix3 temp;
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        temp.m[i][j] = m[i][j] - b.m[i][j];
      }
    }
    return temp;
  }

  FORCEINLINE Matrix3
    Matrix3::operator*(const Matrix3& b) {
    Matrix3 temp = ZERO;
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        for (UInt32 k = 0; k < 3; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    return temp;
  }

  Matrix3&
    Matrix3::operator+=(const Matrix3& b) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        m[i][j] += b.m[i][j];
      }
    }
    return *this;
  }

  Matrix3&
    Matrix3::operator-=(const Matrix3& b) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        m[i][j] -= b.m[i][j];
      }
    }
    return *this;
  }

  Matrix3&
    Matrix3::operator*=(const Matrix3& b) {
    Matrix3 temp = ZERO;
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        for (UInt32 k = 0; k < 3; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    *this = temp;
    return *this;
  }

  Matrix3&
    Matrix3::operator+=(const float& value) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        m[i][j] += value;
      }
    }
    return *this;
  }

  Matrix3&
    Matrix3::operator-=(const float& value) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        m[i][j] -= value;
      }
    }
    return *this;
  }

  Matrix3&
    Matrix3::operator*=(const float& value) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        m[i][j] *= value;
      }
    }
    return *this;
  }

  Matrix3&
    Matrix3::operator/=(const float& value) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        m[i][j] /= value;
      }
    }
    return *this;
  }

  Matrix3
    Matrix3::operator==(const Matrix3& b) {
    for (UInt32 i = 0; i < 3; ++i) {
      for (UInt32 j = 0; j < 3; ++j) {
        if (m[i][j] != b.m[i][j]) { return false; }
      }
    }
    return true;
  }

}