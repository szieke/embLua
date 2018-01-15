#include "global.h"
#include "hwInit.h"
#include <intrinsics.h>
#include "uart_api.h"
#include "string.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "luaInterface.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luaHelper.h"

#include "script1.h"
#include "script2.h"
#include "helperScript.h"

#include "printFreeHeap.h"




#define USER_TASK1_STACK_SIZE (configMINIMAL_STACK_SIZE + (1100 / sizeof(StackType_t)))
#define USER_IF_TASK1_PRIO (tskIDLE_PRIORITY + 5)

#define USER_TASK2_STACK_SIZE (configMINIMAL_STACK_SIZE + (1100 / sizeof(StackType_t)))
#define USER_IF_TASK2_PRIO (USER_IF_TASK1_PRIO - 1)

static TaskHandle_t userTask1;
static TaskHandle_t userTask2;


void sendBlocking(const char* buffer)
{
  uint16_t sentBytes = 0;
  uint16_t size = strlen(buffer);

  while (sentBytes < size)
  {
    while (!(HWREG8(HW_UART_DEBUG_UART + OFS_UCAxIFG) & UCTXIFG))
      ;
    HWREG8(HW_UART_DEBUG_UART + OFS_UCAxTXBUF) = buffer[sentBytes];
    sentBytes++;
  }
}


void vApplicationMallocFailedHook(void)
{
  sendBlocking("vApplicationMallocFailedHook\n");
  while (1)
  {
    _nop();
  }
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{

  sendBlocking("vApplicationStackOverflowHook\n");
  while (1)
  {
    _nop();
  }
}

static void UserTask1(void* pvParameters)
{
  (void) pvParameters;

  lua_State *L = luaL_newstate(); /* create state */
  if (L == NULL)
  {
    lua_writestringWithoutsize("cannot create state: not enough memory");
    while(1){vTaskDelay(1000);}
  }
  else
  {
    luaL_openlibs(L); /* open standard libraries */

    if (dostring(L, helperScript, "helperScript") != LUA_OK)
    {while(1){vTaskDelay(1000);};}

    lua_gc(L, LUA_GCSTOP, 0); //stop the automatic garbage collection

    vTaskDelay(10);

    while (1)
    {

      if (dostring(L, script1, "script1") != LUA_OK)
      {while(1){vTaskDelay(1000);};}

      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage

      lua_writeline();
      printMinimumFreeHeap();

      lua_writestringWithoutsize("\n****************************************************************************\n");

    }

    //lua_close(L);

  }

}

QueueHandle_t g_scriptSemaphore = NULL;

static void UserTask2(void* pvParameters)
{
  (void) pvParameters;

  lua_State *L = luaL_newstate(); /* create state */
  if (L == NULL)
  {
    lua_writestringWithoutsize("cannot create state: not enough memory");
    while(1){vTaskDelay(1000);}
  }
  else
  {

    luaL_openlibs(L); /* open standard libraries */

    if (dostring(L, helperScript, "helperScript") != LUA_OK)
    {while(1){vTaskDelay(1000);};}

    lua_gc(L, LUA_GCSTOP, 0); //stop the automatic garbage collection

    while (1)
    {
      xSemaphoreTakeRecursive(g_scriptSemaphore, portMAX_DELAY);

      if (dostring(L, script2, "script2") != LUA_OK)
      {while(1){vTaskDelay(1000);};}

      lua_gc(L, LUA_GCCOLLECT, 0); //collect garbage

      xSemaphoreGive(g_scriptSemaphore);
    }

    //lua_close(L);

  }

}


void main(void)
{
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer.

  taskDISABLE_INTERRUPTS()
  ;

  initializeMainComponents();

  g_scriptSemaphore = xSemaphoreCreateMutex();

  xTaskCreate(UserTask1, "UserTask1", USER_TASK1_STACK_SIZE, NULL, USER_IF_TASK1_PRIO, &userTask1);
  xTaskCreate(UserTask2, "UserTask2", USER_TASK2_STACK_SIZE, NULL, USER_IF_TASK2_PRIO, &userTask2);

  //Starte alle Tasks und Timer.
  vTaskStartScheduler();


}
