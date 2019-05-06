/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_gameobject.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/03/24 2019
 * @brief GameObject implementation
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#include "tb_gameobject.h"

namespace toyboxSDK {

  TString
  GameObject::getID() {
    return m_ID;
  }

  void
  GameObject::setID(TString id) {
    m_ID = id;
  }

  Vector3
  GameObject::getPosition() {
    return m_position;
  }

  void
  GameObject::setPosition(Vector3 pos) {
    m_position = pos;
  }
}

