/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_gameobject.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/10 2019
 * @brief GameObject for game implementation
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_core_prerequisites.h"

#include <tb_vector3.h>


namespace toyboxSDK {
  class GameObject
  {
  public:
    /**
     * Default constructor
     */
    GameObject();

    /**
     * Default destructor
     */
    ~GameObject();

   public:

    /**
     * Position of gameObject in world
     */
    Vector3 m_position;
  };
  
}

