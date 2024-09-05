#include <app_ringBuffer.h>
#include "drv_uartApi.h"
#include "drv_canApi.h"
#include "drv_clockApi.h"
#include "drv_cacheApi.h"
#include "app_global.h"

#include "stm32l5xx_hal.h"
#include "string.h"

#define lua_c

#include "lprefix.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "helper/luaHelper.h"

#include "script1.h"
#include "script2.h"
#include "helperScript.h"
#include "printFreeHeap.h"

extern uint32_t cmdIf_getTimeDiffFromNow(uint32_t startTime);

int main(void)
{

  char* mesg;
  HAL_Init();

  drv_clockInit();
  drv_cacheInit();
  drv_uartInit();

  mesg = "Start\n";
  lua_writestring(mesg, strlen(mesg));

  //Create the lua state.
  lua_State *L = luaL_newstate();
  if (L == NULL)
  {
    mesg = "cannot create state: not enough memory";
    lua_writestring(mesg, strlen(mesg));
    while (1);
  }
  else
  {
    //Open the standard libraries.
    luaL_openlibs(L);

    //Load the helper script.
    if (dostring(L, helperScript, "helperScript") != LUA_OK)
    {
      while (1);
    }

    while (1)
    {
      //Execute script 1.
      if (dostring(L, script1, "script1") != LUA_OK)
      {
        while (1);
      }

      //Collect garbage.
      lua_gc(L, LUA_GCCOLLECT, 0);

      lua_writeline();

      //Execute script 2.
      if (dostring(L, script2, "script2") != LUA_OK)
      {
        while (1);
      }

      lua_writeline();
      printMinimumFreeHeap();

      mesg = "\n****************************************************************************\n";
      lua_writestring(mesg, strlen(mesg));

    }//while (1)
  }

}

/*************************************************************************/

void fault_reportError(fault_error_t error)
{
  char buffer[16];
  lua_writestringerror("\n\nfatal error: ", sizeof("\n\nfatal error: ") - 1);
  snprintf(buffer, 16, "%x", error);
  lua_writestringerror(buffer, strlen(buffer))
  lua_writestringerror("\n\n", sizeof("\n\n") - 1);
}

