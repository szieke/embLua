

#ifndef LUAINTERFACE_H_
#define LUAINTERFACE_H_

#include "lua.h"

//
#define LUA_INTERFACE_LIBS {"uc", luaopen_uc}


LUAMOD_API int (luaopen_uc) (lua_State *L);



#endif /* LUAINTERFACE_H_ */
