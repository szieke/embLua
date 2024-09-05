
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
	char* mesg;
  /* ---- Disable maskable interrupts ---- */
  clrpsw_i();

  /* ---- Stopping the peripherals which start operations  ---- */
  R_INIT_StopModule();

  /* ---- Initialization of the clock ---- */
  R_INIT_Clock();

  trace_initializeUart();

  setpsw_i();

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

#ifdef __cplusplus
void abort(void)
{
  lua_writestring("\nabort\n", sizeof("\nabort\n") - 1);
}
#endif

/* End of File */

