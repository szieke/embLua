# embLua
The aim of embLua (includes the complete Lua 5.3.4 core) is to provide C/C++ developers the possibility to extend their microcontroller/embedded device with scripting functionality. 
The main focus of embLua is the fast/easy integration in custom projects and new platforms. Therefore only a subset of Lua modules and global functions are available:
- table module
- string module
- math module
- global functions: assert, collectgarbage, error, getmetatable, ipairs, next, pairs, pcall, print, rawequal, rawlen, rawget, rawset, select, setmetatable, tonumber, tostring, type, xpcall, loadstring, load

Note: The math module is only included if the define LUA_WITH_MATH is defined in luaProjectConfig.h.

## embLua requirements
To run scripts which do something 'meaningful' embLua needs:
- RAM: 1k Stack, 11k Heap
- ROM (not optimized code): 120 on MSP430, 90k on Renesas
- C-Library

## Integrating embLua
To integrate embLua in a custom project:
- copy the source files embLua/lua into your project
- create the embLua config file luaInterface.h (an example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject)


### luaProjectConfig.h
This file is used to configure embLua (an example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject). 

#### Memory management 
embLua can be configured to use the build in memory management functions (embLua/lua/helper/luaHeap.c) or to use project specific memory functions. To configure the memory management following defines are used:

- DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS: If the standard memory management functions shall be used then this define must be set, if it is not set then the memory functions in lus/helper/luaHeap.c are used
- LUA_HEAP_SIZE: The heap size if the standard memory management shall not be used (DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS is not defined)
- LUA_MEM_ENTER_CRITICAL_SECTION and LUA_MEM_LEAVE_CRITICAL_SECTION: These defines are used to protect the memory management calls if several Lua instances in different tasks are created (only neded if DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS is not defined)
- luaMallocFunction(size) and luaFreeFunction(pointer): These defines must be set if the build in memory management shall not be used (*DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS*)

#### Other configuration options 
- luaPointerSize_t: If the size of size_t is not the pointer size on the current platform then this define must be set (example: on the MSP430 the pointer size is 32Bit but size_t is only 16Bit)
- LUA_DECIMAL_POINT: The "radix character" (decimal point) used by Lua is per default '.'. With this define this character can be changed.
- LUA_WITH_MATH: If the math module shall be included then this define must be set.
- WITH_PRECOMPILED_SCRIPTS: If Lua shall execute precompiled script (luac.exe) then this define must be set.

## Embedding lua scripts
To execute a Lua script the function dostring (embLua/lua/helper/luaHelper.c) can be used. This function can execute scipts wich resides in the RAM or in the ROM.

### Compiling a Lua script into ROM
For this purpose embLua/lua/convertToHeader.lua can be used. This script converts a Lua script into a header file. To run this script the header file must be included and dofile with the name of the header file must be called (an example can be found in embLua/examples/MSP430F6747A_FreeRTOS/main.c).

Under embLua/examples/MSP430F6747A_FreeRTOS/luaProject/Scripts a batch file can be found (convertToHeader.bat) which shows the usage of convertToHeader.lua.

## Extending embLua
To extend embLua with custom module/functions the define LUA_INTERFACE_LIBS can be used. An example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject/luaInterface.h
