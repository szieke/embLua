

#ifndef LUA_HELPER_LUAHEAP_H_
#define LUA_HELPER_LUAHEAP_H_

#ifndef DONT_USE_LUA_HEAP_MANAGEMENT_FUNCTIONS
void *luaMalloc( size_t xWantedSize );
void luaFree( void *pv );
size_t LuaGetFreeHeapSize( void );
size_t LuaGetMinimumEverFreeHeapSize(void);
#endif

#endif /* LUA_HELPER_LUAHEAP_H_ */
