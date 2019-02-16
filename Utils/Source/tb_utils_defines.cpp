/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_defines.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/02/16 2019
 * @brief implementation of some basic math things
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/

#include "tb_utils_defines.h"

namespace toyboxSDK {
  const float TBMath::PI = static_cast<float>(std::atan(1) * 4);

  const float TBMath::PIHALF = static_cast<float>(std::atan(1) * 2);

  const float TBMath::PIQUARTER = static_cast<float>(std::atan(1));

  const float TBMath::EULER = static_cast<float>(std::pow(1 + 1 / 1000000, 1000000));

  const float TBMath::RAD2DEG = static_cast<float>(180.0f / TBMath::PI);

  const float TBMath::DEG2RAD = static_cast<float>(TBMath::PI / 180.0f);
}