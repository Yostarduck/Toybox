/*||偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|*/
/**
 * @file tileObject.h
 * @author Marco "Swampy" Millan
 * @date 2019/01/04 2019
 * @brief A nice and simple tile that can have different textures and covers the
 * world
 * 
 */
/*||偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|偽院|*/
#pragma once
#include <tileResources.h>

namespace toyboxSDK {
   /**
    * tileObject
    * Description:
    * 	A Tile Object to floor the world
    * Sample usage:
    * 	
    */
  class tileObject
  {
   public:
    /**
     * Default constructor
     */
    tileObject() = default;
  
    /**
     * Default destructor
     */
    ~tileObject() {}

   public:

    /**
     * Tile ID
     */
    int m_ID;

    /**
     * tile sprite
     */
    sf::Sprite m_sprite;

    /**
     * Position in Grid Map
     */
    sf::Vector2f m_position;

  };
}

