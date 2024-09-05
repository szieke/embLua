/*!
 * \file drv_canApi.h
 * \brief Contains the API declarations of the CAN driver.
 * \author Stefan Zieker
 */

#ifndef INCLUDE_DRV_CANAPI_H_
#define INCLUDE_DRV_CANAPI_H_

#include "app_global.h"

/**
 * Initializes the CAN module.
 */
void drv_canInit(void);

/**
 * Receives a CAN message (from the receive fifos).
 *
 * @param canId The CAN ID of the message.
 * @param buffer [out] The buffer for the data of the message.
 * @param numberOfReceivedBytes [out] The number of received bytes.
 * @param timeout The timeout for receiving a message (ms).
 * @param traceReceivedData TRUE if the received data shall be traced.
 */
bool_t drv_canReceiveMessage(uint32_t* canId, uint8_t *buffer, uint32_t* numberOfReceivedBytes, uint32_t timeout, bool_t traceReceivedData);

/**
 * Sends a CAN message.
 *
 * @param canId The CAN ID of the message.
 * @param data The data of the CAN message.
 * @param length The data length.
 * @param flushRxQueue TRUE if all received CAN messages shall be flushed before the message is sent.
 */
bool_t drv_canSendMessage(uint32_t canId, uint8_t *data, uint32_t length, bool_t flushRxQueue);

/**
 * Flushes all received CAN message.
 */
void drv_canFlushRx();

#endif /* INCLUDE_DRV_CANAPI_H_ */
