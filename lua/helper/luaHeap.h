

#ifndef LUA_HELPER_LUAHEAP_H_
#define LUA_HELPER_LUAHEAP_H_

/**
 * Allocates memory.
 *
 * @param xWantedSize The size to allocate.
 */
void *luaMallocFunction( size_t xWantedSize );

/**
 * Releases a memory block.
 *
 * @param pv Pointer to the memory block.
 */
void luaFreeFunction( void *pv );

/**
 * Returns the remaining space on the heap.
 */
size_t luaGetFreeHeapSize( void );

/**
 * Returns min. remaining space on the heap.
 */
size_t luaGetMinimumEverFreeHeapSize(void);

#endif /* LUA_HELPER_LUAHEAP_H_ */
