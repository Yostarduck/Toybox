/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_component.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/03/29 2019
 * @brief Component member definition
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/

#include "tb_component.h"

namespace toyboxSDK {





//////////////////////////////////////////////////////////////////////////
  //Component member definition
//////////////////////////////////////////////////////////////////////////


  Frame
  Animation2D::atSecond(float interval) {
    
    float rate = m_duration / m_sequence.size();

    float mod = std::fmodf(interval, m_duration);

    int actualFrame = static_cast<int>(mod / rate);
    return m_sequence[actualFrame];
  }

  Frame
  Animation2D::atFrame(int atPosition) {

    int mod = atPosition % m_sequence.size();
    return m_sequence[mod];

  }


  Animation2D
  Animation8Way::get(DIR direction) {
    return m_facing[direction];
  }

  Animation2D
  Animation4Way::get(DIR direction) {
    return m_facing[direction];
  }

}