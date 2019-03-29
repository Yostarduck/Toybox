/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_unit.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/23 2019
 * @brief Unit is a movable, controllable or living object ingame
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_gameobject.h"


namespace toyboxSDK {
  class Unit : public GameObject
  {
  public:
    /**
     * Default constructor
     */
    Unit() = default;

    /**
     * Default destructor
     */
    ~Unit() = default;

 
  };
  
}


