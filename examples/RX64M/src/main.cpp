
#include <machine.h>
#include <stdint.h>
#include "iodefine.h"
#include "r_init_clock.h"
#include "r_init_stop_module.h"

#include "trace_uart.h"

#define lua_c

#include "lprefix.h"
#include <string.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "luaHelper.h"

#include "script1.h"
#include "script2.h"
#include "helperScript.h"
#include "printFreeHeap.h"

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

  trace_write("Start\n", sizeof("Start\n"), TRUE);

  lua_State *L = luaL_newstate(); /* create state */
  if (L == NULL)
  {
    lua_writestringWithoutsize("cannot create state: not enough memory");
    while (1)
      ;
  }
  else
  {
    luaL_openlibs(L); /* open standard libraries */
    
    if (dostring(L, helperScript, "helperScript") != LUA_OK)
    {
      while (1)
      {
      };
    }

    while (1)
    {

      if (dostring(L, script1, "script1") != LUA_OK)
      {
        while (1)
          ;
      }

      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage

      lua_writeline();

      if (dostring(L, script2, "script2") != LUA_OK)
      {
        while (1)
          ;
      }

      lua_writeline();
      printMinimumFreeHeap();

      lua_writestringWithoutsize("\n****************************************************************************\n");

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

