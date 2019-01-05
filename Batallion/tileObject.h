/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tileObject.h
 * @author Marco "Swampy" Millan
 * @date 2019/01/04 2019
 * @brief A nice and simple tile that can have different textures and covers the
 * world
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once


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

  };
}

