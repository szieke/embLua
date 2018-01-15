
#include "printFreeHeap.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luaHeap.h"
#include "luaconf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printFreeHeap(char* message)
{
  char buffer[16];

#ifdef DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS
  //Use the FreeRTOS function.
  size_t size = xPortGetFreeHeapSize();
#else
  size_t size = LuaGetFreeHeapSize();
#endif

  snprintf(buffer, 16, "%i", size);
  lua_writestringWithoutsize(message);
  lua_writestringWithoutsize(buffer);
  lua_writeline();
}

void printMinimumFreeHeap()
{
  char buffer[16];

#ifdef DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS
  //Use the FreeRTOS function.
  size_t size = xPortGetMinimumEverFreeHeapSize();
#else
  size_t size = LuaGetMinimumEverFreeHeapSize();
#endif

  snprintf(buffer, 16, "%i", size);
  lua_writestringWithoutsize("minimum free heap: ");
  lua_writestringWithoutsize(buffer);
  lua_writeline();
}
