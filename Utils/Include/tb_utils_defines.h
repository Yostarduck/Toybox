/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_utils_defines.h
 * @author Marco "Swampy" Millan
 * @date 2019/02/16 2019
 * @brief some basic math definitions needed in the utils random
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_defines.h"

#include <math.h>
#include <algorithm>
#include <limits>
namespace toyboxSDK {
  class TBMath
  {
  public:
    /**
     * Constant PI      Equivalent to atan(1) * 4
     */
    static const float PI;

    /**
     * Constant PI / 2  Equivalent to atan(1) * 2
     */
    static const float PIHALF;

    /**
     * Constant PI / 4  Equivalent to atan(1) * 1
     */
    static const float PIQUARTER;

    /**
     * Constant EULER
     */
    static const float EULER;

    /**
     * DEGREES equivalent to 1 RAD
     */
    static const float RAD2DEG;

    /**
     * RADS equivalent to 1 DEGREE
     */
    static const float DEG2RAD;

    /**
     * A very big number
     */
    static const float BIG;

    /**
     * A very small number
     */
    static const float SMALL;

    /**
     * A very very very small number, can be used as an error margen
     */
    static const float EPSILON;
  };
}