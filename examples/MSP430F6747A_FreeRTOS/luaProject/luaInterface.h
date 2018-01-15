

#ifndef LUAINTERFACE_H_
#define LUAINTERFACE_H_

#include "lua.h"

//
#define LUA_INTERFACE_LIBS {"uc", luaopen_uc}


LUAMOD_API int (luaopen_uc) (lua_State *L);



#include "uart_api.h"
#define lua_writestring(s,l)   uart_write(UART_DEBUG, (uint8_t*)s, l, TRUE);
#define lua_writestringWithoutsize(s)   uart_write(UART_DEBUG, (uint8_t*)s, strlen(s), TRUE);
#define lua_writeline()        lua_writestring("\n", 1);
#define lua_writestringerror(s,l)   uart_write(UART_DEBUG, (uint8_t*)s, l, TRUE);

#endif /* LUAINTERFACE_H_ */
