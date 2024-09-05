
#ifndef LUAPROJECTCONFIG_H_
#define LUAPROJECTCONFIG_H_

#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include "helper/luaHeap.h"
#include <semaphore.h>


//This function is called by Lua if it cannot handle an occurred error.
void luaAbort(void);

//If the math module shall be included then this define must be defined.
//define LUA_WITH_MATH

//The heap size if the standard memory management shall not be used .
#define LUA_HEAP_SIZE (40000)

//These defines are used to protect the memory management calls if several Lua instances in different threads are created.
extern sem_t memorySema;
#define LUA_MEM_ENTER_CRITICAL_SECTION()  sem_wait(&memorySema);
#define LUA_MEM_LEAVE_CRITICAL_SECTION()  sem_post(&memorySema);


//Function for printing text from lua.
extern uint32_t uart_send(const void *buffer, uint32_t size);
#define lua_writestring(string,length)       uart_send((uint8_t*) string, length);
#define lua_writeline()                      lua_writestring("\n", 1);
#define lua_writestringerror(string,length)  lua_writestring(string, length);


#endif /* LUAPROJECTCONFIG_H_ */
