
#ifndef LUAPROJECTCONFIG_H_
#define LUAPROJECTCONFIG_H_

#include <limits.h>
#include <stddef.h>
#include <string.h>
#include "helper/luaHeap.h"
#include "drv_uartApi.h"


//This function is called by Lua if it cannot handle an occurred error.
void luaAbort(void);

//If the math module shall be included then this define must be defined.
//define LUA_WITH_MATH

//The heap size if the standard memory management shall not be used .
#define LUA_HEAP_SIZE (20000)

//These defines are used to protect the memory management calls if several Lua instances in different threads are created.
#define LUA_MEM_ENTER_CRITICAL_SECTION()
#define LUA_MEM_LEAVE_CRITICAL_SECTION()

//Function for printing text from lua.
#define lua_writestring(string,length)       drv_uartSendData((uint8_t*) string, length);
#define lua_writeline()                      lua_writestring("\n", 1);
#define lua_writestringerror(string,length)  lua_writestring(string, length);


#endif /* LUAPROJECTCONFIG_H_ */
