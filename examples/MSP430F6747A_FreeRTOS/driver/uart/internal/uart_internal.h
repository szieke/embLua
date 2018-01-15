/**
 * Includes the internal functions and defines of the uart driver.
 */

#ifndef _UART_INTERNAL_H_
#define _UART_INTERNAL_H_

#include "global.h"
#include "uart_api.h"
#include "semphr.h"

/*!< Contains all data for one UART.*/
typedef struct
{
    uint8_t* receive;/*!< Receive buffer (ring buffer).*/
    uint16_t receiveSize;/*!< The size of recive.*/
    uint8_t* transmit; /*!< The transmit buffer (ring buffer).*/
    uint16_t transmitSize;/*!< The size of transmit.*/
    uint16_t bytesInReceiveBuffer; /*!< The number of byte in the receive buffer.*/
    uint16_t receiveHead; /*!< The head of the receive buffer.*/
    uint16_t receiveTail; /*!< The tail of the receive buffer.*/
    uint16_t transmitHead; /*!< The head of the transmit buffer.*/
    uint16_t transmitTail; /*!< The tail of the transmit buffer.*/
    uint16_t bytesInTransmitBuffer; /*!< The number of byte in the trasmit buffer.*/
    uint16_t uart; /*!< UART address.*/
    uint16_t port;/*!< UART port.*/
    uint16_t txPin; /*!< UART Tx pin.*/
    uint16_t rxPin; /*!< UART Rx pin.*/
    bool_t transmitIsInProgress; /*!< TRUE if the UART is transmitting.*/
    QueueHandle_t apiSemaphore; /*!< UART semaphore.*/

} uartData_t;

/**
 * Checks if data is available in the UART receive buffer.
 *
 * \param uartData The UART instance data.
 * \result The number of bytes which can be read.
 */
uint16_t uart_peakInternal(CONST uartData_t* CONST uartData,  bool_t enableReceiveInterrupt);

/**
 * \brief Reads data from a UART receive buffer.
 *
 * \param uartData The UART instance data.
 * \param buffer The read buffer.
 * \param bufferSize The size of buffer (the number of bytes which shall be read).
 * \result The number of bytes which could be read.
 */
uint16_t uart_readInternal(uartData_t* CONST uartData, uint8_t buffer[], CONST uint16_t bufferSize);

/**
 * \brief UART ISR.
 *
 * \param uartData The UART instance data.
 */
void uart_irqHandlerInternal(uartData_t* CONST uartData);

/**
 * Checks if data can be written to the buffer of a UART.
 *
 * \param uartData The UART instance data.
 * \result The number of bytes which can be sent.
 */
uint16_t uart_writePossibleInternal(CONST uartData_t* CONST uartData);

/**
 * Writes data to the buffer of a UART and start the transmitting.
 *
 * \param uartData The UART instance data.
 * \param buffer The buffer which contains the data.
 * \param bufferSize The size of buffer.
 */
uint16_t uart_writeInternal(uartData_t* CONST uartData, CONST uint8_t buffer[], CONST uint16_t bufferSize);

#endif
