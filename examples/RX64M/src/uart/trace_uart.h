/**
 * Includes the API functions and defines of the uart driver.
 */

#ifndef _UART_API_H_
#define _UART_API_H_


#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!< Die Groesse des Trace-UART Transmit-Buffer.*/
#define TRACE_TRANSMIT_BUFFER_SIZE 1024

/*!< Die Groesse des Debug-UART Receive-Buffer.*/
#define TRACE_RECEIVE_BUFFER_SIZE 1024

#ifndef bool_t
typedef unsigned char bool_t;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define TRACE_RECEIVE_EVENT 0x10000000

void trace_initializeUart();

void trace_rxInterrupt(uint8_t data);

void trace_txInterrupt(void);


/**
 * Schreibt Daten in einen UART Buffer und startet ggf. das Senden der Daten.
 *
 * \param buffer Der Datenpuffer.
 * \param bufferSize Die Groesse von buffer.
 * \param waitUntilAllBytesAreInBuffer TRUE wenn solange in der Funktion gewartet werden soll
 * bis alle Daten in den UART-Speicher geschrieben werden konnte.
 * \result Die Anzahl der Bytes, die geschrieben werden konnten.
 */
uint32_t trace_write(const uint8_t buffer[], const uint32_t bufferSize, bool_t waitUntilAllBytesAreInBuffer);

/**
 * Prueft, ob Daten zum Lesen vorhanden sind.
 *
 * \result Die Anzahl der Bytes, die gelesen werden koennen.
 */
uint32_t trace_peak(void);

/**
 * \brief Liest Daten von einem UART.
 *
 * \param buffer Der Datenpuffer.
 * \param bufferSize Die Groesse von buffer.
 * \result Die Anzahl der Bytes, die gelesen werden konnten.
 */
uint32_t trace_read(uint8_t buffer[], const uint32_t bufferSize);

#ifdef __cplusplus
  }
#endif

#endif
