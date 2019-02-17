/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_math_defines.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/16 2019
 * @brief math implementation of the const values
 *
 */
 /*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/

#include "tb_math_defines.h"

namespace toyboxSDK {

const float Math::PI = static_cast<float>(std::atan(1) * 4);

const float Math::PIHALF = static_cast<float>(std::atan(1) * 2);

const float Math::PIQUARTER = static_cast<float>(std::atan(1));

const float Math::EULER = static_cast<float>(std::pow(1 + 1 / 1000000, 1000000));

const float Math::RAD2DEG = static_cast<float>(180.0f / Math::PI);

const float Math::DEG2RAD = static_cast<float>(Math::PI / 180.0f);

}