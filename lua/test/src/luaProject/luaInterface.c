
#include "luaInterface.h"

#include "../../../lua.h"
#include "../../../lauxlib.h"
#include "../../../lualib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//This function is called by Lua cannot handle an occured error.
void luaAbort(void)
{
  lua_writestringerror("luaAbort", sizeof("luaAbort"));
  while(1){}
}


int func1(lua_State *L)
{
  size_t size;
  const char *message = luaL_checklstring(L, 1, &size);

  const char* msg = "C func1:";
  lua_writestring(msg, strlen(msg));
  lua_writestring(message, size);
  lua_writeline();

  //Cleanup the stack.
  int n = lua_gettop(L);
  lua_pop(L, n);


  //Create the result.
  lua_pushinteger(L, 1);
  lua_pushinteger(L, 2);

  return 2;//2 results
}

int func2(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);

  lua_settop(L, 1);


  const char* msg = "C func2:";
  lua_writestring(msg, strlen(msg));


  lua_gettable(L, 1);

  //Add dummy value for the last lua_pop.
  lua_pushnil(L);

  while(lua_next(L, -2))
  {
    lua_Integer b = (int)lua_tonumber(L, -1);
    char value = 48 + b;
    lua_writestring(" ", 1);
    lua_writestring(&value, 1);
    lua_pop(L, 1);
  }

  lua_writeline();

  //Cleanup the stack.
  int n = lua_gettop(L);
  lua_pop(L, n);


  //Create the result.
  lua_newtable(L);
  lua_pushinteger(L, 5);
  lua_rawseti(L, -2, 0);
  lua_pushinteger(L, 9);
  lua_rawseti(L, -2, 1);

  return 1;
}

int counterFunction(lua_State *L)
{
  static uint32_t counter = 0;

  counter++;
  lua_pushinteger(L, counter);
  return 1;
}


static const luaL_Reg uC_funcs[] = {
  {"func1", func1},
  {"func2", func2},
  {"counter", counterFunction},
  {NULL, NULL}
};


LUAMOD_API int luaopen_uc (lua_State *L) {
  luaL_newlib(L, uC_funcs);
  return 1;
}
