/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * System Name  : RX64M initialization example
 * File Name    : main.c
 * Version      : Ver 1.00
 * Device       : R5F564MLDxFC(RX64M Group)
 * Abstract     : Program example of RX64M initialization
 * Tool-Chain   : e2 studio Version 5.4.0.015
 *              : C/C++ Compiler Package for RX Family (V.2.06.00)
 * OS           : not use
 * H/W Platform : Renesas Starter Kit for RX64M
 * Description  : Initialize the clock and non-existent ports.
 * Limitation   : none
 *******************************************************************************/
/*******************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 31.05.2017 1.00     First Release
 *******************************************************************************/

/*******************************************************************************
 Includes <System Includes> , "Project Includes"
 *******************************************************************************/
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

}
#endif

/* End of File */

