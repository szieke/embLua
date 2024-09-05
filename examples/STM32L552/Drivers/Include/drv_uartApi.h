/*!
 * \file drv_uartApi.h
 * \brief Contains the API declarations of the UART driver.
 * \author Stefan Zieker
 */

#ifndef INCLUDE_DRV_UARTAPI_H_
#define INCLUDE_DRV_UARTAPI_H_

#include <app_global.h>

/**
 * Initializes the UART.
 */
void drv_uartInit(void);

/**
 * Sends data with the UART.
 *
 * @param buffer The data buffer.
 * @param size The size of the data buffer.
 */
uint32_t drv_uartSendData(uint8_t *buffer, uint32_t size);

/**
 * Reads received data.
 *
 * @param buffer [out] The data buffer.
 * @param size The size of the data buffer.
 */
uint32_t drv_uartGetData(uint8_t *buffer, uint32_t size);

#endif /* INCLUDE_DRV_UARTAPI_H_ */
