/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file cpp
 * @author Marco "Swampy" Millan
 * @date 2018/10/17 2018
 * @brief Random member implementation
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/

#include "tb_random.h"

namespace toyboxSDK {

float 
RandomZ::next() {
  float res =
    (1.0f / std::sqrt(2.0f*TBMath::PI*m_deviation)) *
    std::exp(-((m_seed - std::pow(m_mean,2.0f)) / 2.0f*m_deviation));
  m_seed = res;
  return res;

}

float
RandomMC::next() {
  
  float res = 0;
  
  return res;
}

void
RandomBM::initialize() {
  m_seed = static_cast<float>(time(0));

  m_z1 = static_cast<float>(time(0));
  m_z2 = static_cast<float>(time(0));
}

float
RandomBM::next() {

  if (m_inquery) {
    m_inquery = false;
    return m_z1;
  }

  float z1 = std::sqrt(-2.0f*std::log(m_z2))*std::cos(2.0f*TBMath::PI*m_z2);
  float z2 = std::sqrt(-2.0f*std::log(m_z1))*std::cos(2.0f*TBMath::PI*m_z1);

  m_z1 = z2;
  m_z2 = z1;

  m_inquery = true;
  return m_z2;
}

}

