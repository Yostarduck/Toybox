/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file nauMatrix3.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/06 2019
 * @brief Matrix3x3 implementation
 *
 */
 /*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#include "tb_matrix3x3.h"

namespace toyboxSDK {

const Matrix3x3 Matrix3x3::ZERO = Matrix3x3(0);

const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3(MAT3::kONE);

Matrix3x3::Matrix3x3(const Matrix3x3& other) {
  _m = other._m;
}

Matrix3x3::Matrix3x3(Int32 value) {
  memset(this, 0, sizeof(Matrix3x3));

  if (MAT3::kZERO != value) {
    _m.m00 = _m.m11 = _m.m22 = 1.0f;
  }
}

void
Matrix3x3::identity() {
  *this = IDENTITY;
}

void
Matrix3x3::zero() {
  *this = ZERO;
}

void
Matrix3x3::transposed() {
  Matrix3x3 temp = *this;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      m[i][j] = temp.m[j][i];
    }
  }
}

void
Matrix3x3::setValues(float value) {
  memset(&_m, static_cast<int>(value), sizeof(_m));
}

void
Matrix3x3::inverse() {
  float det = determinant();

  if (det == 0.0f) {
    printf("Matrix cant be inverse");
    return;
  }

  Matrix3x3 temp = *this;
  m[0][0] =  ((temp.m[1][1] * temp.m[2][2]) - (temp.m[1][2] * temp.m[2][1]));
  m[1][0] = -((temp.m[1][0] * temp.m[2][2]) - (temp.m[1][2] * temp.m[2][0]));
  m[2][0] =  ((temp.m[1][0] * temp.m[2][1]) - (temp.m[1][1] * temp.m[2][0]));
  m[0][1] = -((temp.m[0][1] * temp.m[2][2]) - (temp.m[0][2] * temp.m[1][2]));
  m[1][1] =  ((temp.m[0][0] * temp.m[2][2]) - (temp.m[0][2] * temp.m[2][0]));
  m[2][1] = -((temp.m[0][0] * temp.m[2][1]) - (temp.m[0][1] * temp.m[2][0]));
  m[0][2] =  ((temp.m[0][1] * temp.m[1][2]) - (temp.m[0][2] * temp.m[1][1]));
  m[1][2] = -((temp.m[0][0] * temp.m[1][2]) - (temp.m[0][2] * temp.m[1][0]));
  m[2][2] =  ((temp.m[0][0] * temp.m[1][1]) - (temp.m[0][1] * temp.m[1][0]));

  *this /= det;
}

float
Matrix3x3::determinant() {
  return (((m[0][0] * m[1][1] * m[2][2]) +
           (m[1][0] * m[2][1] * m[0][2]) +
           (m[2][0] * m[0][1] * m[1][2])) -

          ((m[0][2] * m[1][1] * m[2][0]) +
           (m[1][2] * m[2][1] * m[0][0]) +
           (m[2][2] * m[0][1] * m[1][0])));
}

FORCEINLINE Matrix3x3
Matrix3x3::operator+(const Matrix3x3& b) {
  Matrix3x3 temp;
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      temp.m[i][j] = m[i][j] + b.m[i][j];
    }
  }
  return temp;
}

FORCEINLINE Matrix3x3
Matrix3x3::operator-(const Matrix3x3& b) {
  Matrix3x3 temp;
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      temp.m[i][j] = m[i][j] - b.m[i][j];
    }
  }
  return temp;
}

FORCEINLINE Matrix3x3
Matrix3x3::operator*(const Matrix3x3& b) {
  Matrix3x3 temp = ZERO;
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      for (UInt32 k = 0; k < 3; ++k) {
        temp.m[i][j] += m[i][k] * b.m[k][j];
      }
    }
  }
  return temp;
}

Matrix3x3&
Matrix3x3::operator+=(const Matrix3x3& b) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      m[i][j] += b.m[i][j];
    }
  }
  return *this;
}

Matrix3x3&
Matrix3x3::operator-=(const Matrix3x3& b) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      m[i][j] -= b.m[i][j];
    }
  }
  return *this;
}

Matrix3x3&
Matrix3x3::operator*=(const Matrix3x3& b) {
  Matrix3x3 temp = ZERO;
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

Matrix3x3&
Matrix3x3::operator+=(const float& value) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      m[i][j] += value;
    }
  }
  return *this;
}

Matrix3x3&
Matrix3x3::operator-=(const float& value) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      m[i][j] -= value;
    }
  }
  return *this;
}

Matrix3x3&
Matrix3x3::operator*=(const float& value) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      m[i][j] *= value;
    }
  }
  return *this;
}

Matrix3x3&
Matrix3x3::operator/=(const float& value) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      m[i][j] /= value;
    }
  }
  return *this;
}

bool
Matrix3x3::operator==(const Matrix3x3& b) {
  for (UInt32 i = 0; i < 3; ++i) {
    for (UInt32 j = 0; j < 3; ++j) {
      if (m[i][j] != b.m[i][j]) { return false; }
    }
  }
  return true;
}

}