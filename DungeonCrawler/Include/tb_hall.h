/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_hall.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/18 2019
 * @brief Hall class for connecting rooms
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once
#include <tb_core_prerequisites.h>
#include <tb_gameobject.h>

namespace toyboxSDK {
   /**
    * Hall
    * Description:
    *   A hall made for connecting rooms in the dungeon
    * Sample usage:
    *   Hall connects two rooms
    */
  class Hall : public GameObject
  {
  public:
    Hall() = default;
    ~Hall() = default;
  };
  
}


