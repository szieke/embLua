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
    luaL_openlibs(L); /* open standard libraries */

    char buffer[32];
    uint32_t counter = 0;

    while (1)
    {

      lua_writestringWithoutsize("\nStart: ");
      sprintf(buffer, "%i", counter++);
      lua_writestringWithoutsize(buffer);
      lua_writeline();

      if (dostring(L, calls, "calls") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
      lua_writeline();

      if (dostring(L, closure, "closure") != LUA_OK)
      {
        while (1)
          ;
      }
      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage
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

