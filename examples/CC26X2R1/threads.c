/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <semaphore.h>
#include <ti/drivers/UART2.h>
#include <pthread.h>

/* Driver configuration */
#include "ti_drivers_config.h"


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


extern uint32_t uart_send(const void *buffer, uint32_t size);


sem_t threadSema;


/* Stack size in bytes */
#define THREADSTACKSIZE 2500


void * threadFunction(char* name)
{
    char* mesg;

    //Create the lua state.
    lua_State *L = luaL_newstate();
    if (L == NULL)
    {
      mesg = "cannot create state: not enough memory";
      lua_writestring(mesg, strlen(mesg));
      while (1)
      {
          sleep(1);
      }

    }
    else
    {
       //Open the standard libraries.
      luaL_openlibs(L);

      //Load the helper script.
      if (dostring(L, helperScript, "helperScript") != LUA_OK)
      {
        while (1)
        {
            sleep(1);
        }
      }

      while (1)
      {
        //Wait for the other thread.
        sem_wait(&threadSema);

        lua_writestring(name, strlen(name));
        lua_writeline();

        //Execute script 1.
        if (dostring(L, script1, "script1") != LUA_OK)
        {
          while (1)
          {
              sleep(1);
          }
        }

        //Collect garbage.
        lua_gc(L, LUA_GCCOLLECT);

        lua_writeline();

        //Execute script 2.
        if (dostring(L, script2, "script2") != LUA_OK)
        {
          while (1)
          {
              sleep(1);
          }
        }

        lua_writeline();
        printMinimumFreeHeap();

        mesg = "\n****************************************************************************\n";
        lua_writestring(mesg, strlen(mesg));

        sem_post(&threadSema);

        usleep(1000 * 100);

      }//while (1)
    }
}
void *thread1(void *arg0)
{
    return threadFunction("thread1");
}

void *thread2(void *arg0)
{
    return threadFunction("thread2");
}

void createThread1(void)
{
    pthread_t thread;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;

    sem_init(&threadSema, 0, 0);
    sem_post(&threadSema);

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc                    = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
    {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, thread1, NULL);
    if (retc != 0)
    {
        /* pthread_create() failed */
        while (1) {}
    }
}

void createThread2(void)
{
    pthread_t thread;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc                    = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
    {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, thread2, NULL);
    if (retc != 0)
    {
        /* pthread_create() failed */
        while (1) {}
    }
}
