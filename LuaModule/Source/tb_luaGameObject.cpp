/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_luaGameObject.cpp
 * @author Marco "Swampy" Millan
 * @date 2019/03/10 2019
 * @brief Lua GameObject register implementations
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/

#include "tb_luaGameObject.h"

namespace toyboxSDK {
  int 
  createGameObject(lua_State* L) {
    GameObject** userdata = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
    *userdata = new GameObject();
    luaL_getmetatable(L, "lGameObject");
    lua_setmetatable(L, -2);
    return 1;
  }

  int
  copyGameObject(lua_State* L) {
    GameObject* copy = getGameObject(L, 1);
    GameObject** userdata = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
    userdata = &copy;
    luaL_getmetatable(L, "lGameObject");
    lua_setmetatable(L, -2);
    return 1;
  }

  GameObject*
  getGameObject(lua_State* L, int n) {
    return *(GameObject**)luaL_checkudata(L, n, "lGameObject");
  }

  int
  destroyGameObject(lua_State* L) {
    GameObject* c = getGameObject(L, 1);
    delete c;
    return 0;
  }

  void
  registerGameObject(lua_State* L) {
    luaL_Reg unitRegs[] = {
    {"new",  createGameObject},
    {"copy", copyGameObject},
    {NULL, NULL}
    };
    luaL_newmetatable(L, "lGameObject");
    luaL_setfuncs(L, unitRegs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    lua_setglobal(L, "GameObject");
  }

}
