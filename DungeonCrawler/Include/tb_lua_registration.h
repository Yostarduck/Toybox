/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_lua_registration.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/18 2019
 * @brief This is for creating all objects with lua registrations
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include "tb_luaGameObject.h"

namespace toyboxSDK {
  
  static void
  registerObjects(lua_State* L) {
    
    registerGameObject(L);

  }
  
}

