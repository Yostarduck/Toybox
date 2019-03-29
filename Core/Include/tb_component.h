/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_component.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/24 2019
 * @brief Its a property residing in the gameObject
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_core_prerequisites.h"

#include<SFML/Graphics.hpp>
#include <tb_vector2.h>
#include <vector>


namespace toyboxSDK {

  struct Component
  {
   public:
     /**
      * Default constructor
      */
    Component() = default;

    /**
     * Default destructor
     */
    ~Component() = default;


  };

  struct Frame
  {
  public:
    /**
     * Default constructor
     */
    Frame() = default;

    /**
     * Default destructor
     */
    ~Frame() = default;

    /**
     * Default parameter constructor
     */
    Frame(Vector2 size, Vector2 pos = { 0,0 }, bool flipped = false)
      : m_position(pos),
        m_size(size),
        m_isFlipped(flipped) {}
   
  private:
    /**
     * Position in the sprite Sheet
     */
    Vector2 m_position;

    /**
     * Cutting size of the sprite
     */
    Vector2 m_size;

    /**
     * Should the sprite render flipped over X?
     */
    bool m_isFlipped;

    /**
     * The frame as a Sprite object
     */
    sf::Sprite m_sprite;
  };


  struct TB_CORE_EXPORT Animation2D : public Component
  {
  public:
    /**
     * Default constructor
     */
    Animation2D() = default;
  
    /**
     * Default destructor
     */
    ~Animation2D() = default;
    
    /**
     * @brief Returns the frame at the given second
              If the given second is higher than the duration of the animation
              It will wrap it i.e: atSecond(13) with duration 10 will give at second 3
     * @param the float second for that frame
     * @return the frame
     *
     */
    Frame
    atSecond(float interval);

    /**
     * @brief Returns the frame at the given position of the array
              If the position is higher than the given number, it will wrap
              i.e: atPosition(5) with sequence.size() == 3 will give you frame 2
     * @param the frame to get
     * @return the frame
     *
     */
    Frame
    atFrame(int atPosition);

  private:
    /**
     * Animation sequence made by a set of frames from the same sprite Sheet or different
     */
    std::vector<Frame> m_sequence;

    /**
     * The duration of the animation in seconds
     */
    float m_duration;
  };

  /**
   * A structure to control an 8 Way animation sequence, like in RTS where you
   * can go in 8 directions
   */
  struct Animation8Way 
  {
   public:

    /**
     * Defines the Access to the Animation array
     */
    enum DIR
    {
      NORTH     = 0,
      NWEST     = 1,
      WEST      = 2,
      SWEST     = 3,
      SOUTH     = 4,
      SEAST     = SWEST,          //Its the same as Southwest, just flipped
      EAST      = WEST,           //Its the same as West, just flipped
      NEAST     = NWEST,          //Its the same as NorthWest, just flipped
      NUM_DIRS  = 5
    };

    /**
     * Default Constructor
     */
    Animation8Way() = default;

    /**
     * Default destructor
     */
    ~Animation8Way() = default;

    /**
     * @brief Returns the animation based up on the direction
     * @param the DIRECTION its facing (in 8 way system) i.e: NORTH, NORTHWEST, etc
     * @return the animation
     *
     */
    Animation2D
    get(DIR direction);


   private:

    /**
     * The animation array
     */
    Animation2D m_facing[8];
  };

  /**
   * A structure to control a 4 Way animation sequence, like in GameBoy games where you
   * can go in 4 directions
   */
  struct Animation4Way
  {
   public:

     /**
      * Defines the Access to the Animation array
      */
    enum DIR
    {
      NORTH     = 0,
      WEST      = 1,
      SOUTH     = 2,
      EAST      = WEST,         //Its the same as West, just flipped
      NUM_DIRS  = 3
    };

    /**
     * Default Constructor
     */
    Animation4Way() = default;

    /**
     * Default destructor
     */
    ~Animation4Way() = default;

    /**
     * @brief Returns the animation based up on the direction
     * @param the DIRECTION its facing (in 4 way system) i.e: NORTH, SOUTH, etc
     * @return the animation
     *
     */
    Animation2D
    get(DIR direction);


   private:

    /**
     * The animation array
     */
    Animation2D m_facing[4];


  };

}
