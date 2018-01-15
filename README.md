# embLua
The aim of embLua is to provide C/C++ developers the possibility to extend their microcontroller/embedded device with scripting functionality. The main focus of embLua is the fast/easy integration in custom projects and new platforms. Therefore only a subset of Lua modules and functions are available:
- table module
- string module
- global functions: assert, collectgarbage, error, getmetatable, ipairs, next, pairs, pcall, print, rawequal, rawlen, rawget, rawset, select, setmetatable, tonumber, tostring, type, xpcall

## embLua requirements
To run scripts which do something 'meaningful' embLua needs:
- 13 Heap
- 1k stack
- 132k ROM (not optimized code)
- C-Library

## Integrating embLua
To integrate embLua in a custom project:
- copy the source files embLua/lua into your project
- create the embLua config file luaInterface.h (an example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject)


### luaProjectConfig.h
This files is used to config  embLua (an example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject). 

#### Memory management 
embLua can be configured to use the build in memory management functions (embLua/lua/helper/luaHeap.c) or to use project specific memory functions. To configure the memory management following defines are used:

- LUA_HEAP_SIZE: The heap size if the build in memory management shall be used
- LUA_MEM_ENTER_CRITICAL_SECTION and LUA_MEM_LEAVE_CRITICAL_SECTION: These defines are used protect the memory management calls if several Lua instance in different tasks are created
- DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS: If the build in memory function shall not be used then this define must be set.
- luaMallocFunction(size) and luaFreeFunction(pointer): These defines must be set if the build in memory management shall not be used (*DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS*)
- luaPointerSize_t: If the size of size_t is not the pointer size on the current platform then this define must be set (example: on the MSP430 the pointer size is 32Bit but size_t is only 16Bit)

## Embedding lua scripts
To execute a Lua script the function dostring (embLua/lua/helperluaHelper.c) can be used. This function can execute scipts wich resides in the RAM or in the ROM.

### Compiling a Lua script into ROM
For this purpose embLua/lua/convertToHeader.lua can be used. This script converts a Lua script into a header file. To run this script the header file must be included and dofile with the name of the header file must be called (an example can be found in embLua/examples/MSP430F6747A_FreeRTOS/main.c).

Under embLua/examples/MSP430F6747A_FreeRTOS/luaProject/Scripts a batch file can be found (convertToHeader.bat) which shows the usage of convertToHeader.lua.

**Extending embLua**
To extend embLua with custom module/functions the define LUA_INTERFACE_LIBS can be used. An example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject/luaInterface.h
