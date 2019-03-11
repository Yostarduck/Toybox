/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_room.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/11 2019
 * @brief Room gameObject in the dungeon. Is a room procedurally generate
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include <tb_core_prerequisites.h>
#include <tb_gameobject.h>

namespace toyboxSDK {
  class Room : public GameObject
  {
  public:
    /**
     * Default constructor
     */
    Room() = default;

    /**
     * Default destructor
     */
    ~Room() = default;
  };
  
}

