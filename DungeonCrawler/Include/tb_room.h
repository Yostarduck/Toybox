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

#include "SFML/Graphics.hpp"

namespace toyboxSDK {
  using namespace sf;


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

    
    RectangleShape m_northBoundary;
    RectangleShape m_southBoundary;

    RectangleShape m_westBoundary;
    RectangleShape m_eastBoundary;


  };
  
  

}

