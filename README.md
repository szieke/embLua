# embLua
The aim of embLua is to provide C/C++ developers the possibility to extend their microcontroller/embedded device with scripting functionality. The main focus of embLua is the fast/easy integration in custom projects and new platforms. Therefore only a subset of Lua modules and functions are available:
- table module
- string module
- global functions: assert, collectgarbage, error, getmetatable, ipairs, next, pairs, pcall, print, rawequal, rawlen, rawget, rawset, select, setmetatable, tonumber, tostring, type, xpcall

## Integrating embLua
To integrate embLua in a custom project:
- copy the source files embLua/lua into your project
- create the embLua config file luaInterface.h (an example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject)


## luaProjectConfig.h
This files is used to config  embLua (an example can be found under embLua/examples/MSP430F6747A_FreeRTOS/luaProject). 

### Memory management 
embLua can be configured to use the build in memory management functions (embLua/lua/helper/luaHeap.c) or to use the memory management functions set by the project. If the build in memory function shall not be used then the DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS must be set.

## Embedding lua scripts


**Extending embLua**
ToDo
