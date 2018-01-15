/**
 * Includes the API functions and defines of the uart driver.
 */

#ifndef _UART_API_H_
#define _UART_API_H_

#include "global.h"

/*!< Debug/Trace UART baudrate*/
#define UART_DEBUG_BAUDRATE 115200


/*!< Debug-UART transmit buffer size.*/
#define UART_DEBUG_TRANSMIT_BUFFER_SIZE 256

/*!< Debug-UART rransmit buffer size.*/
#define UART_DEBUG_RECEIVE_BUFFER_SIZE 256



/**
 * Contains all UART instances.
 */
typedef enum
{
  UART_DEBUG = 0, /*!< Debug/Trace UART.*/
  UART_END_MARKER /*!< Marks the end of the enum.*/

} uartInstance_t;

/**
 * Initializes the Debug UART.
 */
void uart_initializeDebug(void);


/**
 * Sets the  baudrate for one UART.
 *
 * \param instance [in] The UART instance.
 * \param baudrate [in] The baudrate.
 */
void uart_setBaudrate(CONST uartInstance_t instance, uint32_t baudrate);

/**
 * Blocks until all data for one UART have been sent.
 *
 * \param instance [in] The UART instance.
 */
void uart_blockUntilAllSent(CONST uartInstance_t instance);


/**
 * Checks if data can be written to the buffer of a UART.
 *
 * \param instance The UART instance.
 * \result The number of bytes which can be sent.
 */
uint16_t uart_writePossible(CONST uartInstance_t instance);

/**
 * Writes data to the buffer of a UART and start the transmitting.
 *
 * \param instance The UART instance.
 * \param buffer The buffer which contains the data.
 * \param bufferSize The size of buffer.
 * \param waitUntilAllBytesAreInBuffer TRUE if this function shall block until all bytes are in the UART buffer.
 * \result The number of bytes which have been written.
 */
uint16_t uart_write(CONST uartInstance_t instance, CONST uint8_t buffer[], CONST uint16_t bufferSize,
    bool_t waitUntilAllBytesAreInBuffer);

/**
 * Checks if data is available in the UART receive buffer.
 *
 * \param instance The UART instance.
 * \result The number of bytes which can be read.
 */
uint16_t uart_peak(CONST uartInstance_t instance);

/**
 * \brief Reads data from a UART receive buffer.
 *
 * \param instance The UART instance.
 * \param buffer The read buffer.
 * \param bufferSize The size of buffer (the number of bytes which shall be read).
 * \result The number of bytes which could be read.
 */
uint16_t uart_read(CONST uartInstance_t instance, uint8_t buffer[], CONST uint16_t bufferSize);

#endif
