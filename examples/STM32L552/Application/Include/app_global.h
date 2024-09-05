#ifndef __GLOBAL_H
#define __GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "luaProjectConfig.h"

#define UC_UID_ADDRESS 0x0BFA0590

typedef unsigned char bool_t;

/*! \brief Current major SW version of the SW. */
#define RLS_SW_VERSION_MAJOR ((uint8_t)1U)

/*! \brief Current minor SW version of the SW */
#define RLS_SW_VERSION_MINOR ((uint8_t)2U)

#ifndef NULL
/*! \brief NULL */
#define NULL ((void*)0)
#endif

#ifndef FALSE
/*! \brief FALSE */
#define FALSE ((bool_t)0U)
#endif

#ifndef TRUE
/*! \brief TRUE */
#define TRUE (((bool_t)!FALSE))
#endif

#ifndef STATIC
#define STATIC static
#endif /* ifndef STATIC */

#ifndef CONST
#define CONST const
#endif /* ifndef CONST */

typedef enum
{
  FAULT_ERROR_CACHE_INIT_1 = 0x00000000,
  FAULT_ERROR_CACHE_INIT_2 = 0x00000001,

  FAULT_ERROR_UART_GET_DATA = 0x80000000,
  FAULT_ERROR_UART_INIT_1 = 0x80000001,
  FAULT_ERROR_UART_INIT_2 = 0x80000002,
  FAULT_ERROR_UART_INIT_3 = 0x80000003,

  FAULT_ERROR_CLOCK_INIT_1 = 0x80000100,
  FAULT_ERROR_CLOCK_INIT_2 = 0x80000101,
  FAULT_ERROR_CLOCK_INIT_3 = 0x80000102,

  FAULT_ERROR_CAN_INIT_1 = 0x80000205,
  FAULT_ERROR_CAN_INIT_2 = 0x80000206,
  FAULT_ERROR_CAN_INIT_3 = 0x80000206,
  FAULT_ERROR_CAN_INVALID_SIZE_1 = 0x80000206,

}fault_error_t;

/**
 * Reports an error.
 *
 * @param error The error.
 */
void fault_reportError(fault_error_t error);


#endif /* __GLOBAL_H */
