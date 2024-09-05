
#include "printFreeHeap.h"

#include "../../../lua/lua.h"
#include "../../../lua/lauxlib.h"
#include "../../../lua/lualib.h"
#include "../../../lua/helper/luaHeap.h"
#include "../../../lua/luaconf.h"
#include "luaInterface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printFreeHeap(char* message)
{
  char buffer[16];
  size_t size = luaGetFreeHeapSize();

  snprintf(buffer, 16, "%i", size);
  lua_writestring(message, strlen(message));
  lua_writestring(buffer, strlen(buffer));
  lua_writeline();
}

void printMinimumFreeHeap()
{
  char buffer[16];
  size_t size = luaGetMinimumEverFreeHeapSize();

  snprintf(buffer, 16, "%i", size);
  lua_writestring("minimum free heap: ", sizeof("minimum free heap: ") - 1);
  lua_writestring(buffer, strlen(buffer));
  lua_writeline();
}
