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
#include "tb_component.h"

#include <tb_vector3.h>


namespace toyboxSDK {
  class TB_CORE_EXPORT GameObject
  {
  public:
    /**
     * Default constructor
     */
    GameObject() = default;

    /**
     * Default destructor
     */
    ~GameObject() = default;

    /**
     * Gets the position of the gameObject
     */
    Vector3 
    getPosition();

    /**
     * @brief Sets the position of the gameObject
     * @param new Position
     * @return 
     *
     */
    void
    setPosition(Vector3 pos);

    /**
     * @brief Gets the position of the Object
     * @param 
     * @return the string ID
     *
     */
    TString
    getID();

    /**
     * @brief Sets the string ID of the object
     * @param new ID
     * @return 
     *
     */
    void
    setID(TString id);

   private:

    /**
     * Position of gameObject in world
     */
    Vector3 m_position;

    /**
     * object ID
     */
    TString m_ID;
  };
  
}

