

#ifndef LUAHELPER_H_
#define LUAHELPER_H_


#include "lua.h"

#ifdef __cplusplus
  extern "C" {
#endif

/**
 * Executes a lua script.
 *
 * @param L The lua state.
 * @param s The script.
 * @param name The name of the script.
 * @return LUA_OK on success.
 */
int dostring (lua_State *L, const char *s, const char *name);

#ifdef __cplusplus
  }
#endif
#endif /* LUAHELPER_H_ */
