

#ifndef LUAINTERFACE_H_
#define LUAINTERFACE_H_

#include "../../../lua.h"

//
#define LUA_INTERFACE_LIBS {"uc", luaopen_uc}


LUAMOD_API int (luaopen_uc) (lua_State *L);


#include "trace_uart.h"
#define lua_writestring(s,l)   trace_write((uint8_t*) s, l, TRUE)
#define lua_writestringWithoutsize(s)   trace_write((uint8_t*) s, strlen(s), TRUE)
#define lua_writeline()        trace_write((uint8_t*) "\n", 1, TRUE)
#define lua_writestringerror(s,l)   trace_write((uint8_t*) s, l, TRUE)


#endif /* LUAINTERFACE_H_ */
