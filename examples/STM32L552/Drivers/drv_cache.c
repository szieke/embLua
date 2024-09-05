/*!
 * \file drv_cacheApi.c
 * \brief Contains the cache driver.
 * \author Stefan Zieker
 */

#include <app_global.h>

#include "stm32l5xx_hal.h"

/****************************************************************************************/

void drv_cacheInit(void)
{
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    fault_reportError(FAULT_ERROR_CACHE_INIT_1);
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    fault_reportError(FAULT_ERROR_CACHE_INIT_2);
  }
}
