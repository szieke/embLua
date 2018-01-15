
#include "RTOS.h"
#include "BSP.h"
#include "stdio.h"
#include "string.h"

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

static OS_STACKPTR int StackHP[2048], StackLP[2048]; /* Task stacks */
static OS_TASK TCBHP, TCBLP; /* Task-control-blocks */

static void HPTask(void)
{
  while (1)
  {
    BSP_ToggleLED(0);
    OS_Delay(250);
  }
}

int counter = 0;
char buffer[32];
static void LPTask(void)
{



  lua_writestringWithoutsize("Start1");


  lua_State *L = luaL_newstate();  /* create state */
  if (L == NULL)
  {
    lua_writestringWithoutsize("cannot create state: not enough memory");
  }
  else
  {
    luaL_openlibs(L);  /* open standard libraries */

    if (dostring(L, helperScript, "helperScript") != LUA_OK)
    {while(1){OS_Delay(1000);};}

    while(1)
    {

      if(dostring (L, script1, "script1") != LUA_OK)
      {while(1){OS_Delay(1000);};}

      lua_gc (L, LUA_GCCOLLECT, 0);//collect garbage


      lua_writeline();

      if(dostring (L, script2, "script2") != LUA_OK)
      {while(1){OS_Delay(1000);};}

      lua_writeline();
      printMinimumFreeHeap();

      lua_writestringWithoutsize("\n****************************************************************************\n");

      BSP_ToggleLED(1);

    }
  }
}

/*********************************************************************
 *
 *       main()
 */
int main(void)
{
  OS_IncDI(); /* Initially disable interrupts  */
  OS_InitKern(); /* Initialize OS                 */
  OS_InitHW(); /* Initialize Hardware for OS    */
  BSP_Init(); /* Initialize LED ports*/


  /* You need to create at least one task before calling OS_Start() */
  OS_CREATETASK(&TCBHP, "HP Task", HPTask, 100, StackHP);
  OS_CREATETASK(&TCBLP, "LP Task", LPTask, 50, StackLP);
  OS_Start()
  ; /* Start multitasking            */
  return 0;
}

/****** End Of File *************************************************/
