/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_luaGameObject.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/10 2019
 * @brief Game Object with Lua implementations
 * https://gist.github.com/roxlu/00974d35f64c5c5be8d6 this is a reference
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_lua_prerequisites.h"

#include <tb_gameobject.h>

namespace toyboxSDK {

  /**
   * Creates a GameObject
   */
  int
  createGameObject(lua_State* L);

  /**
   * copies a GameObject and all its parameters
   */
  int
  copyGameObject(lua_State* L);

  /**
   * Gets a gameObject from the stack
   */
  GameObject*
  getGameObject(lua_State* L, int n);

  /**
   * Allows to destroy a gameObject
   */
  int
  destroyGameObject(lua_State* L);

  /**
   * @brief THIS WILL ADD THE GAMEOBJECT TO THE LUA CONTEXT. 
   *        YOU WONT DO SHIT IF THIS IS NOT IN THE REGISTRATIONS
   * @param lua_state* stack
   * @return 
   *
   */
  void
  registerGameObject(lua_State* L);
}