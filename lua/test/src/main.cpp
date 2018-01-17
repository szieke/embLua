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
#include "gc.h"
#include "literals.h"
#include "locals.h"
#include "nextvar.h"
#include "pm.h"
#include "sort.h"
#include "strings.h"
#include "vararg.h"

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

  lua_State *L;

  char buffer[32];
  uint32_t counter = 0;

  while (1)
  {

    L = luaL_newstate(); /* create state */
    if (L == NULL)
    {
      lua_writestringWithoutsize("cannot create state: not enough memory");
      while (1)
        ;
    }
    luaL_openlibs(L); /* open standard libraries */

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

    /**********************gc.lua************************************************/

    if (dostring(L, gc, "gc") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();



    /***Reopen Lua***********************/
    lua_close(L);

    L = luaL_newstate(); /* create state */
    if (L == NULL)
    {
      lua_writestringWithoutsize("cannot create state: not enough memory");
      while (1)
        ;
    }
    luaL_openlibs(L); /* open standard libraries */

    /**********************literals.lua************************************************/

    if (dostring(L, literals, "literals") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    /**********************locals.lua************************************************/

    if (dostring(L, locals, "locals") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    /**********************nextvar.lua************************************************/

    if (dostring(L, nextvar, "nextvar") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    /**********************pm.lua************************************************/

    if (dostring(L, pm, "pm") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    /**********************sort.lua************************************************/

    if (dostring(L, sort, "sort") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    /**********************strings.lua************************************************/

    if (dostring(L, strings, "strings") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    /**********************vararg.lua************************************************/

    if (dostring(L, vararg, "vararg") != LUA_OK)
    {
      while (1)
        ;
    }
    lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
    lua_writeline();

    printMinimumFreeHeap();
    lua_writeline();

    lua_close(L);

  }

}

#ifdef __cplusplus
void abort(void)
{
  lua_writestringWithoutsize("\nabort\n");

}
#endif

/* End of File */

