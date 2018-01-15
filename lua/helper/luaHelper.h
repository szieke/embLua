

#ifndef LUAHELPER_H_
#define LUAHELPER_H_


#include "lua.h"

#ifdef __cplusplus
  extern "C" {
#endif

int dostring (lua_State *L, const char *s, const char *name);

#ifdef __cplusplus
  }
#endif
#endif /* LUAHELPER_H_ */
