#include <machine.h>
#include <stdint.h>
#include "iodefine.h"
#include "r_init_clock.h"
#include "r_init_stop_module.h"

#include "trace_uart.h"

#define lua_c

#include "../../lprefix.h"
#include <string.h>

#include "../../lua.h"
#include "../../lauxlib.h"
#include "../../lualib.h"

#include "../../helper/luaHelper.h"

#include "printFreeHeap.h"
#include "calls.h"
#include "closure.h"
#include "constructs.h"
#include "errors.h"
#include "events.h"

void main(void);

#ifdef __cplusplus
extern "C"
{
void abort(void);
}
#endif

void main(void)
{
  /* ---- Disable maskable interrupts ---- */
  clrpsw_i();

  /* ---- Stopping the peripherals which start operations  ---- */
  R_INIT_StopModule();

  /* ---- Initialization of the clock ---- */
  R_INIT_Clock();

  trace_initializeUart();

  setpsw_i();

  lua_State *L = luaL_newstate(); /* create state */
  if (L == NULL)
  {
    lua_writestringWithoutsize("cannot create state: not enough memory");
    while (1)
      ;
  }
  else
  {


    char buffer[32];
    uint32_t counter = 0;

    luaL_openlibs(L); /* open standard libraries */

    while (1)
    {

      lua_writestringWithoutsize("\nStart: ");
      sprintf(buffer, "%i", counter++);
      lua_writestringWithoutsize(buffer);
      lua_writeline();

      /**********************calls.lua************************************************/
      if (dostring(L, calls, "calls") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
      lua_writeline();

      /**********************closure.lua************************************************/

      if (dostring(L, closure, "closure") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
      lua_writeline();

      /**********************constructs.lua************************************************/

      if (dostring(L, constructs, "constructs") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
      lua_writeline();

      /**********************errors.lua************************************************/

      if (dostring(L, errors, "errors") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
      lua_writeline();



      /**********************events.lua************************************************/

      if (dostring(L, events, "events") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
      lua_writeline();




      printMinimumFreeHeap();
      lua_writeline();

    }
  }
}

#ifdef __cplusplus
void abort(void)
{
  lua_writestringWithoutsize("\nabort\n");

}
#endif

/* End of File */

