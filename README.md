# embLua
The aim of embLua (includes the complete Lua 5.4.7 core) is to provide C/C++ developers the possibility to extend their microcontroller/embedded device with scripting functionality. 
The main focus of embLua is the fast/easy integration in custom projects and new platforms. Therefore only a subset of Lua modules and global functions are available:
- table module
- string module
- global functions: assert, collectgarbage, error, getmetatable, ipairs, load, next, pairs, pcall, print, warn, rawequal, rawlen, rawget, rawset, select, setmetatable, tonumber, tostring, type, type

Note: The math module is only included if the define LUA_WITH_MATH is defined in luaProjectConfig.h.

## embLua requirements
To run scripts which do something 'meaningful' embLua needs:
- TI CC2642R1: 2k stack, 20k Heap, 160k ROM (-O0), 100k ROM (-Oz)
- STM32L552: 2k stack, 20k Heap, 210k ROM (-O0), 120k ROM (-Os)
- Renesas RX64M: 3k stack, 20k Heap, 135k ROM (no optimizations), 95k ROM (max. otimizations)
- C-Library

## Integrating embLua
To integrate embLua in a custom project:
- copy the source files embLua/lua into your project
- create the embLua config file luaInterface.h (an example can be found under embLua/examples/STM32L552/luaProject)


### luaProjectConfig.h
This file is used to configure embLua (an example can be found under embLua/examples/STM32L552/luaProject). 

#### Memory management 
To configure the memory management following defines are used:

- LUA_HEAP_SIZE: The heap size if the standard memory management shall not be used.
- LUA_MEM_ENTER_CRITICAL_SECTION and LUA_MEM_LEAVE_CRITICAL_SECTION: These defines are used to protect the memory management calls if several Lua instances in different threads are created.

#### Other configuration options 
- LUA_DECIMAL_POINT: The "radix character" (decimal point) used by Lua is per default '.'. With this define this character can be changed.
- LUA_WITH_MATH: If the math module shall be included then this define must be set.

#### Needed functions 
- void luaAbort(void): This function is called by Lua if it cannot handle an occurred error.
- lua_writestring(string,length), lua_writeline, lua_writestringerror(string,length): These function are called by lua to print texts.

## Embedding lua scripts
To execute a Lua script the function dostring (embLua/lua/helper/luaHelper.c) can be used. This function can execute scipts wich resides in the RAM or in the ROM.

### Compiling a Lua script into ROM
For this purpose embLua/lua/convertToHeader.lua can be used. This script converts all Lua scripts in a given directory (sript argument) into header files. To run these scripts the header files must be included and dofile with the name of these header files must be called (an example can be found in embLua/examples/STM32L552/main.c).

Under embLua/examples/STM32L552/luaProject/Scripts a batch file can be found (convertToHeader.bat) which shows the usage of convertToHeader.lua.

## Extending embLua
To extend embLua with custom module/functions the define LUA_INTERFACE_LIBS can be used. An example can be found under embLua/examples/STM32L552/luaProject/luaInterface.h
